#include "pch.h"
#include "PGM.h"

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

namespace Renderer
{
	PGM::PGM(const std::shared_ptr<DeviceResources>& devResources) :
		deviceResources(devResources)
	{
	}

	bool PGM::Initialize()
	{		
		pgmShader = std::make_unique<Shader>(deviceResources, "PGMPassThroughVertexShader.cso", "", "PGMGeometryShader.cso");
		pgmShader->SetInputDescriptor(gridPointIED, sizeof(gridPointIED) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		pgmShader->Initialize(sizeof(DefaultCBuffer));
		
		rasterizationShader = std::make_unique<Shader>(deviceResources, "PGMVertexShader.cso", "PGMPixelShader.cso");
		rasterizationShader->SetInputDescriptor(pgmRasterizationIED, sizeof(pgmRasterizationIED) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		rasterizationShader->Initialize(sizeof(DefaultCBuffer));

		// create the SO buffer to get data between PGM and rasterization stages
		auto context = deviceResources->GetD3DDeviceContext();

		Platform::Array<byte>^ GSFile;
		GSFile = LoadShaderFile(pgmShader->gsFileName);

		UINT stride = sizeof(PGM::SOVertex);

		deviceResources->GetD3DDevice()->CreateGeometryShaderWithStreamOutput(
			GSFile->Data, 
			GSFile->Length,
			gridPointSOIED,
			sizeof(gridPointSOIED) / sizeof(D3D11_SO_DECLARATION_ENTRY),
			&stride,
			1,
			D3D11_SO_NO_RASTERIZED_STREAM, // to not pass SO to pixelshader
			NULL,
			pgmShader->geometryshader.GetAddressOf()
			);

		// initialize the number of grid points to use for the sampling camera
		numGridPointsX = 64;
		numGridPointsY = 64;

		// THIS IS JUST FOR TESTING
		primaryRadius = 1.0f;
		sphereWorldPos = Base::Math::VectorInit({ 0.0f, 0.0f, 0.0f });
		// set surface extents here:
		// plane: extent x = FarPlane, y = FarPlane
		// sphere: extent x = radius, y = radius
		// do we just have two extents?  how does this work when you're talking about viewing
		// this object 3-dimensionally?
		// END JUST FOR TESTING

		return true;
	}

	void PGM::SetViewCamera(const Renderer::Camera& c)
	{
		ViewCamera.Set(c);

		// update view camera
		ViewCamera.UpdateMatrices();
	}

	Renderer::Camera PGM::GetViewCamera()
	{
		return ViewCamera;
	}

	void PGM::SetSamplingCamera(const Renderer::Camera& c)
	{
		SamplingCamera.Set(c);

		// calculate extents here for the view camera
		// use extents for the surface here, along with the nadir
		XMVECTOR nadir = XMVectorSet(0, -1, 0, 1);
		XMVECTOR nadir_length = XMVector3Dot(ViewCamera.Position, nadir);
		float n_length;
		Base::Math::VectorGet(nadir_length, &n_length, 0);

		float farplane = ViewCamera.FarPlane;
		float nearplane = ViewCamera.NearPlane;

		float alpha = atan2(farplane, abs(n_length));  // angle of the extent vector
		float fovy = alpha * 2; // max fov possible
		float aspect = 1.0;

		// get the vectors corresponding to a frustum around the surface, where the nadir is the view vector
		nadir = XMVector3Normalize(nadir);
		XMVECTOR view = nadir;
		XMVECTOR right;
		if (!XMVector3Equal(ViewCamera.View, nadir))
			right = XMVector3Cross(ViewCamera.View, nadir);
		else
			right = XMVector3Cross(ViewCamera.Right, nadir);
		XMVECTOR up = XMVector3Cross(view, right);
		
		// how to take into account the rotated view camera?  
		// and how the aspect ratio of it affects the sampling camera?
		// do we simply recreate the full sampling camera from scratch every time?

		// can we do this entirely using fov for the calculations?
		// need to average the nadir and the extents to get the proper view vector
		// (assuming the nadir is an extent vector; else just average the frustum/surface extents

		// keep view vector the same
		// get angle between current top extent & new top extent
		// subtract that angle from fovy and then rotate the view vector by angle/2
		// do the same for right extent
		// recalculate the aspect as fovy/fovx

		// frustum corners:
		XMVECTOR ftl = ViewCamera.TopLeft;
		XMVECTOR ftr = ViewCamera.TopRight;
		XMVECTOR fbl = ViewCamera.BottomLeft;
		XMVECTOR fbr = ViewCamera.BottomRight;
		XMVECTOR ftop = (ftl + ftr ) / 2.0;
		XMVECTOR fbottom = (fbr + fbl) / 2.0;
		XMVECTOR fright = (ftr + fbr) / 2.0;
		XMVECTOR fleft = (ftl + fbl) / 2.0;

		// compute extent vectors
		XMMATRIX R = XMMatrixRotationAxis(right, alpha / 2);
		XMVECTOR etop = XMVector3TransformNormal(view, R);
		R = XMMatrixRotationAxis(right, -alpha / 2);
		XMVECTOR ebottom = XMVector3TransformNormal(view, R);
		R = XMMatrixRotationAxis(up, alpha / 2);
		XMVECTOR eright = XMVector3TransformNormal(view, R);
		R = XMMatrixRotationAxis(up, -alpha / 2);
		XMVECTOR eleft = XMVector3TransformNormal(view, R);

		// variables for testing within a frustum
		XMVECTOR test;
		float dot_test1, dot_test2;
		XMVECTOR sbottom, stop, sleft, sright;

		// test nadir in frustum:
		test = XMVector3Dot(nadir, ftl);
		Base::Math::VectorGet(test, &dot_test1, 0);

		test = XMVector3Dot(nadir, fbr);
		Base::Math::VectorGet(test, &dot_test2, 0);

		if (dot_test1 >= 0 && dot_test2 >= 0)
		{  // nadir is contained within the view frustum
			test = XMVector3Dot(ebottom, ftl);
			Base::Math::VectorGet(test, &dot_test1, 0);

			test = XMVector3Dot(ebottom, fbr);
			Base::Math::VectorGet(test, &dot_test2, 0);
			if (dot_test1 >= 0 && dot_test2 >= 0)
			{   // ebottom is contained within the frustum
				sbottom = ebottom;
			}
			else
			{
				sbottom = fbottom;
			}
		}
		else
		{
			sbottom = nadir;
		}

		test = XMVector3Dot(etop, ftl);
		Base::Math::VectorGet(test, &dot_test1, 0);

		test = XMVector3Dot(etop, fbr);
		Base::Math::VectorGet(test, &dot_test2, 0);
		if (dot_test1 >= 0 && dot_test2 >= 0)
		{   // etop is contained within the frustum
			stop = etop;
		}
		else
		{
			stop = ftop;
		}

		test = XMVector3Dot(eright, ftl);
		Base::Math::VectorGet(test, &dot_test1, 0);

		test = XMVector3Dot(eright, fbr);
		Base::Math::VectorGet(test, &dot_test2, 0);
		if (dot_test1 >= 0 && dot_test2 >= 0)
		{   // eright is contained within the frustum
			sright = eright;
		}
		else
		{
			sright = fright;
		}

		test = XMVector3Dot(eleft, ftl);
		Base::Math::VectorGet(test, &dot_test1, 0);

		test = XMVector3Dot(eleft, fbr);
		Base::Math::VectorGet(test, &dot_test2, 0);
		if (dot_test1 >= 0 && dot_test2 >= 0)
		{   // eleft is contained within the frustum
			sleft = eleft;
		}
		else
		{
			sleft = fleft;
		}

		// need to minimize fovy -> this will minimize viewable volume
		// find extents which contain nadir and have minimum fov

		// from those extents figure out view, up, fov, and aspect
		// possibly set near/far based on the surface as well

		view = XMVector3Normalize((sright + sleft) / 2.0);
		up = XMVector3Normalize((stop - sbottom) / 2.0);
		XMVECTOR tmp_angle = XMVector3AngleBetweenVectors(stop, sbottom);
		Base::Math::VectorGet(tmp_angle, &fovy, 0);
		tmp_angle = XMVector3Length(sright - sleft);
		float width;
		Base::Math::VectorGet(tmp_angle, &width, 0);
		tmp_angle = XMVector3Length(stop - sbottom);
		float height;
		Base::Math::VectorGet(tmp_angle, &height, 0);
		aspect = height / width;

		// need to figure out which frustum vectors are going to be which extents
#if 0
		XMVECTOR u = up;
		XMVECTOR v = view * nearplane;
		XMVECTOR r = right;
		XMVECTOR tl = XMVector3Normalize(v + u - r);
		XMVECTOR tr = XMVector3Normalize(v + u + r);
		XMVECTOR bl = XMVector3Normalize(v - u - r);
		XMVECTOR br = XMVector3Normalize(v - u + r);
		// compute the view/right/up vectors for the new sampling camera
		SamplingCamera.Set(ViewCamera.Position,
			tl, tr, bl, br);
#else
		SamplingCamera.Set(ViewCamera.Position,
			view, up, fovy, aspect, nearplane, farplane);
#endif

		// make extent vectors based on the right and up vectors of the camera?
		// compare extents and determine minimum volume that must be sampled
		// which must also include the nadir
		// use the minimum volume extents to form a sampling camera centered at the
		// position of the view camera

		// update sampling camera
		SamplingCamera.UpdateMatrices();

		// should this be done when updating the sampling camera?
		// should just take the sampling camera and generate points/triangles
		// within its view volume : should keep this within view space;
		// don't require matrix transformation into world space
		// probably need to switch to texture/uav based deferred shading model
		MakeGridPoints();
	}

	Renderer::Camera PGM::GetSamplingCamera()
	{
		return SamplingCamera;
	}

	void PGM::CreateWindowSizeDependentResources()
	{
		Size outputSize = deviceResources->GetOutputSize();
		float aspectRatio = outputSize.Width / outputSize.Height;
		float fovAngleY = XMConvertToRadians(45.0f);

		// This is a simple example of change that can be made when the app is in
		// portrait or snapped view.
		if (aspectRatio < 1.0f)
		{
			fovAngleY *= 2.0f;
		}

		//SamplingCamera.Aspect = aspectRatio;
		//SamplingCamera.FoVY = fovAngleY;
		ViewCamera.Aspect = aspectRatio;
		ViewCamera.FoVY = fovAngleY;

		// Note that the OrientationTransform3D matrix is post-multiplied here
		// in order to correctly orient the scene to match the display orientation.
		// This post-multiplication step is required for any draw calls that are
		// made to the swap chain render target. For draw calls to other targets,
		// this transform should not be applied.

		XMFLOAT4X4 orientation = deviceResources->GetOrientationTransform3D();

		ViewCamera.OrientMatrix = XMLoadFloat4x4(&orientation);

		//SamplingCamera.OrientMatrix = XMLoadFloat4x4(&orientation);

		ViewCamera.UpdateMatrices();
		SetSamplingCamera(ViewCamera);
		//SamplingCamera.UpdateMatrices();
	}

	void PGM::Update()
	{
		// Update the sampling camera here:
		//  * with the view vector along the nadir
		//  * and using the view camera's view vector to 
		//    create the basis vector system for the sampling
		//    camera

		// compute ray-sphere intersection point & sphere intersection normal
		//   gamma1 & gamma 2
		//		* gamma1 = asin((d / r) sin w) - w : first intersection angle from nadir
		//		* gamma2 = -asin((d / r) sin w) - w + pi : second intersection angle from nadir

		// set our new render target object as the active render target
		auto context = deviceResources->GetD3DDeviceContext();
		
		// set up render target buffers for Deferred Rendering:
		//  * Pass 1:
		//		input:
		//		* vertex/index buffers
		//		output:
		//		* positions
		//		* normals
		//	* Pass 2:
		//		input:
		//		* position buffer
		//		* normal buffer
		//		* texture atlas
		//		* height atlas
		//		output:
		//		* rendered scene

		//D3D11_DEPTH_STENCIL_DESC dsd = { 0 };
		//dsd.DepthEnable = FALSE;
		//dsd.StencilEnable = FALSE;
		//ComPtr<ID3D11DepthStencilState> dss;
		//deviceResources->GetD3DDevice()->CreateDepthStencilState(&dsd, dss.GetAddressOf());
		//context->OMSetDepthStencilState(dss.Get(), 0);

		pgmShader->Apply();
		
		// update grid points (per the new sampling viewport)
		//	* calc new extents to edges of sphere for frusta
		//	* calc new distance to planet
		// build grid points into vertex buffer and index buffer
		// send grid points:
		// set the vertex buffer

		// PGM Stage:
		//	* send PGM info (camera info, shape info) in constant buffer
		//	* set grid points vertex buffer / index buffer created for sampling camera to IA stage
		//	* set the stream out vertex buffer to stream out stage

		UINT stride = sizeof(PGM::GridVertex);
		UINT offset = 0;
		// set the vertex buffer (grid points)
		context->IASetVertexBuffers(0, 1, gridvertexbuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(gridindexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// set the stream-out buffer for output from Geometry Shader
		context->SOSetTargets(1,streamOutVertexBuffer.GetAddressOf(),&offset);

		// set the primitive topology
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// update the constant buffers with relevant info for PGM (camera & surface info)
		DefaultCBuffer pgmCbuffer;
		pgmCbuffer.CameraPosition = SamplingCamera.Position;
		pgmCbuffer.ViewVector = SamplingCamera.View;
		
		XMMATRIX matFinal = XMMatrixInverse(NULL, XMMatrixTranspose(SamplingCamera.ViewMatrix));

		pgmCbuffer.matWVP = matFinal;

		context->UpdateSubresource(pgmShader->constantbuffer.Get(), 0, 0, &pgmCbuffer, 0, 0);

		// invoke the shader code for raycasting PGM
		context->DrawIndexed(numIndices, 0, 0);

		pgmShader->Disable();

		//D3D11_DEPTH_STENCIL_DESC dsd2 = { 0 };
		//ComPtr<ID3D11DepthStencilState> dss2;
		//deviceResources->GetD3DDevice()->CreateDepthStencilState(&dsd2,dss2.GetAddressOf());
		//context->OMSetDepthStencilState(dss2.Get(),0);

		// clear stream out stage target from previous stage
		ComPtr<ID3D11Buffer> bufferArray = { 0 };
		context->SOSetTargets(1, bufferArray.GetAddressOf(), &offset);

		SamplingCamera.MakeFrustumBuffers(deviceResources);
	}

	void PGM::Render()
	{
		auto context = deviceResources->GetD3DDeviceContext();

		rasterizationShader->Apply();

		// Rasterization Stage:
		//	* set stream-out stage target to null
		//	* send main camera info & transform from sampling to main camera
		//	* set stream out vertex buffer (from PGM stage) to IA stage

		UINT stride = sizeof(PGM::SOVertex);
		UINT offset = 0;
		// set the vertex buffer to the stream out buffer from the previous stage
		context->IASetVertexBuffers(0, 1, streamOutVertexBuffer.GetAddressOf(), &stride, &offset);

		// set the primitive topology
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// update the constant buffers with relevant info for PGM (camera & surface info)
		DefaultCBuffer rasterizationCbuffer;
		rasterizationCbuffer.CameraPosition = ViewCamera.Position;
		rasterizationCbuffer.ViewVector = ViewCamera.View;

		XMMATRIX matFinal = ViewCamera.ViewMatrix * ViewCamera.ProjMatrix * ViewCamera.OrientMatrix;
		rasterizationCbuffer.matWVP = matFinal;

		// Lighting related
		rasterizationCbuffer.matRotation = XMMatrixIdentity();
		rasterizationCbuffer.DiffuseVector = XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f);
		rasterizationCbuffer.DiffuseColor = XMVectorSet(0.1f, 0.1f, 0.5f, 1.0f);
		rasterizationCbuffer.AmbientColor = XMVectorSet(0.2f, 2.0f, 0.2f, 1.0f);

		context->UpdateSubresource(rasterizationShader->constantbuffer.Get(), 0, 0, &rasterizationCbuffer, 0, 0);

		// Draw the vertices created from the stream-out stage
		context->DrawAuto();

		rasterizationShader->Disable();
	}

	int PGM::GetGridPointXSize()
	{
		return numGridPointsX;
	}

	int PGM::GetGridPointYSize()
	{
		return numGridPointsY;
	}

	void PGM::SetGridPointXSize(int xsize)
	{
		numGridPointsX = xsize > 0 ? xsize : 1;
	}

	void PGM::SetGridPointYSize(int ysize)
	{
		numGridPointsY = ysize > 0 ? ysize : 1;
	}

	void PGM::MakeGridPoints()
	{
		float height = 2 * tan(SamplingCamera.FoVY / 2) * SamplingCamera.NearPlane;
		float width = height * SamplingCamera.Aspect;
		std::vector<PGM::GridVertex> OurVertices;
		// set up vertices based on grid
		for (int j = 0; j < numGridPointsY; j++)
		{
			for (int i = 0; i < numGridPointsX; i++)
			{
				float x, y, z;
				x = (float)(i) / (float)(numGridPointsX - 1)*width - width/2.0;
				y = (float)(j) / (float)(numGridPointsY - 1)*height - height/2.0;
				z = SamplingCamera.NearPlane;
				PGM::GridVertex v = { x, y, z, 1 };
				OurVertices.push_back(v);
			}
		}
		// set up the indices for triangles from the grid
		std::vector<UINT> OurIndices;
		for (int j = 0; j < numGridPointsY - 1; j++)
		{
			for (int i = 0; i < numGridPointsX - 1; i++)
			{
				UINT v0 = i + numGridPointsX*j;
				UINT v1 = v0 + 1;
				UINT v2 = v0 + numGridPointsX;
				UINT v3 = v1 + numGridPointsX;
				// upper triangle
				OurIndices.push_back(v0);
				OurIndices.push_back(v2);
				OurIndices.push_back(v1);
				// lower triangle
				OurIndices.push_back(v1);
				OurIndices.push_back(v2);
				OurIndices.push_back(v3);
			}
		}
		numIndices = (int)OurIndices.size();
		
		// create the vertex buffer
		D3D11_BUFFER_DESC vertexBD = { 0 };
		vertexBD.ByteWidth = sizeof(PGM::GridVertex) * (int)OurVertices.size();
		vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSRD = { OurVertices.data(), 0, 0 };

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&vertexBD, &vertexSRD, &gridvertexbuffer)
			);

		// create the index buffer
		// buffer description
		D3D11_BUFFER_DESC indexBD = { 0 };
		indexBD.ByteWidth = sizeof(UINT) * (int)OurIndices.size();    // indices are stored in UINT values
		indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// subresource data
		D3D11_SUBRESOURCE_DATA indexSRD = { OurIndices.data(), 0, 0 };

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&indexBD, &indexSRD, &gridindexbuffer)
			);


		// create the vertex buffer for stream out between PGM projection and rasterization stages
		D3D11_BUFFER_DESC SOvertexBD;
		SOvertexBD.Usage = D3D11_USAGE_DEFAULT;
		// Need to use indices here because GS will convert from TriangleList to TriangleStrip semantics,
		//   where vertices are duplicated between adjacent triangles
		SOvertexBD.ByteWidth = sizeof(PGM::SOVertex) * (int)OurIndices.size();
		SOvertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		SOvertexBD.CPUAccessFlags = 0;
		SOvertexBD.MiscFlags = 0;

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&SOvertexBD, nullptr, &streamOutVertexBuffer)
			);
	}

	bool PGM::UnInitialize()
	{
		return true;
	}

	void PGM::OnSuspending()
	{

	}

	void PGM::OnResuming()
	{

	}
}