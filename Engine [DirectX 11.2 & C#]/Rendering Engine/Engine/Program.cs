using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using SharpDX;
using SharpDX.Windows;
using SharpDX.DXGI;
using SharpDX.Direct3D11;
// Resolve naming conflicts by explicitly stating the class to use:
using Device = SharpDX.Direct3D11.Device;

namespace Engine
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            #region Direct3D Initialization
            // Create the window to render to
            Form1 form = new Form1();
            form.Text = "D3DRendering - EmptyProject";
            form.Width = 640;
            form.Height = 480;

            // Declare the device and SwapChain vars:
            Device device;
            SwapChain swapChain;

            // Create the device and swapchain:
            Device.CreateWithSwapChain(
                SharpDX.Direct3D.DriverType.Hardware,
                DeviceCreationFlags.None,

                new[] {
                    SharpDX.Direct3D.FeatureLevel.Level_11_1,
                    SharpDX.Direct3D.FeatureLevel.Level_11_0,
                    SharpDX.Direct3D.FeatureLevel.Level_10_1,
                    SharpDX.Direct3D.FeatureLevel.Level_10_0,
                },

                new SwapChainDescription()
                {
                    ModeDescription =
                        new ModeDescription(
                            form.ClientSize.Width,
                            form.ClientSize.Height,
                            new Rational(60, 1),
                            Format.R8G8_B8G8_UNorm),
                    SampleDescription = new SampleDescription(1, 0),
                    Usage = SharpDX.DXGI.Usage.BackBuffer | Usage.RenderTargetOutput,
                    BufferCount = 1,
                    Flags = SwapChainFlags.None,
                    IsWindowed = true,
                    OutputHandle = form.Handle,
                    SwapEffect = SwapEffect.Discard,
                },
                out device, out swapChain
            );

            // Create references for backBuffer and renderTargetView
            var backBuffer = Texture2D.FromSwapChain<Texture2D>(swapChain, 0);
            var renderTargetView = new RenderTargetView(device, backBuffer);
            #endregion


            #region Render Loop
            // Create and run the render loop
            RenderLoop.Run(form, () =>
                {
                    device.ImmediateContext.ClearRenderTargetView(renderTargetView, Color.LightBlue);

                    // Execute rendering commands here...

                    // Present the frame
                    swapChain.Present(0, PresentFlags.None);
                });
            #endregion

            #region Direct3D Cleanup
            // Release the device and any other resources created.
            renderTargetView.Dispose();
            backBuffer.Dispose();
            device.Dispose();
            swapChain.Dispose();
            #endregion

        }
    }
}
