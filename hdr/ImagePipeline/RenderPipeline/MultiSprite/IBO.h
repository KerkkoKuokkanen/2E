
#ifndef INCTANCE_BUFFER_OBJECT_H
# define INCTANCE_BUFFER_OBJECT_H

# include "VAO.h"
# include "box.h"

struct InstanceData {
	t_Point position;
	t_Box texUV;
};

class IBO
{
	private:
		uint32_t maxSize;
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		GLuint texture;
		GLuint instanceEBO;
		GLuint instanceVBO;
		GLuint instanceVAO;
		std::vector<InstanceData> instances;
		void CreateEBO();
	public:
		IBO(uint32_t maxSize, GLuint texture);
		void UpdateInstances(std::vector<InstanceData> &updatedInstances);
		void Draw(float w, float h);
};

void InitIBO();

#endif
