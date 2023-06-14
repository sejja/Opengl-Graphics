//
//	Shader.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _SHADER__H_
#define _SHADER__H_

namespace Core {
	namespace Graphics {
		class  Shader {
		public:
			enum class EType : unsigned char {
				Vertex,
				Fragment
			};
			Shader();
			~Shader();
			Shader(const std::string_view& filename, const EType type);
			bool LoadSource(const std::string_view& filename);
			bool Compile(const std::string_view& filename);
			void SetShaderType(const EType type, const bool createDeviceShader = false);
			bool CreateDeviceShader();

		private:
			friend class ShaderProgram;
			unsigned int mHandle;
			char* Source;
			EType ShaderType;
		};
	}
}

#endif
