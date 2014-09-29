#pragma once

namespace Base
{
	namespace Math
	{
		template < int numRows, typename T >
		class Matrix
		{
		public:
			T vals[numRows*numRows];
			int numVals = numRows * numRows;

			template<typename... Ts>
			Matrix<numRows, T>(Ts&... Args)
			{
				if (sizeof...(Args) == numVals)
				{
					va_list _args;
					va_start(_args, Args);
					for (int i = 0; i < numVals; i++)
					{
						vals[i] = va_arg(_args, T);
					}
				}
			}
		};
	}
}