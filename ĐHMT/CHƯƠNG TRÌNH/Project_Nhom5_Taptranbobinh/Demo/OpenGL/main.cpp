
#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/

// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints]; /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/

point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;

GLfloat thetamanh[20] = { 0 };
GLfloat alpha[3] = { 0, 0, 0 };

mat4 model, model_bia;
GLuint model_loc;
mat4 projection;
GLuint projection_loc;
mat4 view;
GLuint view_loc;
//GLfloat t;
mat4 instance_xe;
mat4 m;
//cua manh
GLfloat pull;
mat4  model1_manh, model2_manh;
// cua linh 
mat4 model_tong;
mat4 model_nguoi, model_linh, model_sung, model_model, instance_tay;
GLfloat beta[] = { 0,0,0,0,0,0,0,0,0,0 };
// cua tâm 
GLfloat tamta[20] = { 0 };
GLfloat matpha[3] = { 0, 0, 0 };
mat4 modelt;
mat4  model2t, model3t, model4t, model5t, modelbanh_kt;
// của duyên 
mat4 ctm_D;

GLfloat speed;

GLfloat goc = 0;
GLfloat upward = 0.05;
// của hoài 
mat4 ctm_H, ctm_cua_H, ctm_banh_H, ctm_kinh_H, instance_H;
GLfloat theta_h[] = { 0, 0, 0 }, yy_h = 0, xx_H = 0;

void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 0.5, 0.0, 1.0); // orange
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry(void)
{
	initCube();
	makeColorCube();
}

void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);


}

void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));


	/*glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);*/

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(254/255, 255/255, 255/255, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}
GLfloat mauAnhSang = 1.0;

mat4  model2, model3, model4, model5,modelbanh_bm;

void paintColor(GLfloat a, GLfloat b, GLfloat c) {
	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
	point4 light_position(0, 0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(mauAnhSang, mauAnhSang, mauAnhSang, 1.0);
	color4 light_specular(0.0, 0.0, 0.0, 1.0);

	color4 material_ambient(1.0, 1.0, 1.0, 1.0);
	color4 material_diffuse(a / 255.0, b / 255.0, c / 255.0, 1.0);
	color4 material_specular(255.0 / 255.0, 255.0 / 255.0, 255 / 255.0, 1.0);
	float material_shininess = 100.0;

	color4 ambient_product = light_ambient   * material_ambient;
	color4 diffuse_product = light_diffuse   * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

}
//XE CUA MANH
void banhxe_manh()
{
	for (float i = 0; i < 360; i = i + 10)
	{
		paintColor(-50, -50, -50);
		m = RotateZ(i) * Translate(0.12, 0, 0) * Scale(0.09, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * model2_manh * m);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}
	for (float i = 0; i < 360; i++)
	{
		paintColor(255, 255, 255);
		m = RotateZ(i) * Translate(0.09, 0, 0) * Scale(0.02, 0.05, 0.11);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * model2_manh * m);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	for (float i = 0; i < 360; i++)
	{
		paintColor(-50, -50, -50);
		m = RotateZ(i) * Translate(0.05, 0, 0) * Scale(0.07, 0.05, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * model2_manh * m);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void canhcua_manh()
{
	//canh cua ben phai
	//cánh chính 
	paintColor(0, 51, 0);
	m = Translate(0.34, 0.02, 0) * Scale(0.3, 0.3, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// khung dọc 1
	m = Translate(0.205, 0.25, 0) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// khung dọc 2
	m = Translate(0.494, 0.25, 0) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//khung cửa sổ ngang 
	m = Translate(0.35, 0.35, 0) * RotateZ(4) * Scale(0.29, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void dauxe_manh()
{
	//truc xe
	paintColor(0, 0, 0);
	m = Translate(1.12, -0.04, 0) * Scale(0.7, 0.1, 0.38);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	paintColor(0, 150, 0);
	m = Translate(0, 0, 0) * Scale(0.4, 0.3, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	paintColor(0, 96, 0);
	m = Translate(0.012, 0.12, 0) * RotateZ(3) * Scale(0.42, 0.1, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//2 thanh gan banh xe
	m = Translate(-0.085, 0, -0.25) * Scale(0.25, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(-0.085, 0, 0.25) * Scale(0.25, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//den xe
	paintColor(255, 255, 153);
	m = Translate(-0.16, -0.05, -0.24) * Scale(0.05, 0.1, 0.08);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(-0.16, -0.05, 0.24) * Scale(0.05, 0.1, 0.08);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//thanh dau xe
	paintColor(-50, -50, -50);
	m = Translate(-0.225, -0.125, 0) * Scale(0.05, 0.05, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(-0.225, -0.175, 0) * Scale(0.05, 0.05, 0.35);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//kinh chan gio
	paintColor(-10, -10, -10);
	//thanh tren dinh
	m = Translate(-0.21, 0.11, 0) * Scale(0.02, 0.02, 0.25);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//2 thanh 2 canh
	m = Translate(-0.21, 0, -0.115) * Scale(0.02, 0.2, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(-0.21, 0, 0.115) * Scale(0.02, 0.2, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//2 thanh cheo gan banh xe
	paintColor(0, 153, 0);
	m = Translate(0.115, -0.07, -0.25) * RotateZ(-40) * Scale(0.22, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(0.115, -0.07, 0.25) * RotateZ(-40) * Scale(0.22, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//noc xe
	paintColor(0, 153, 0);
	m = Translate(0.35, 0.35, 0) * RotateZ(5) * Scale(0.3, 0.02, 0.46);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//chỗ bước lên xe 
	paintColor(0, 153, 0);
	m = Translate(0.35, -0.15, 0) * Scale(0.3, 0.02, 0.63);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mieng dang sau than xe vách ngăn đầu xe và thân xe 
	m = Translate(0.5, 0.12, 0) * Scale(0.02, 0.52, 0.47);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//Ghe xe
	paintColor(0, 76, 153);
	m = Translate(0.45, 0.05, 0.1) * Scale(0.08, 0.35, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(0.38, -0.05, 0.1) * Scale(0.15, 0.1, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(0.45, 0.05, -0.1) * Scale(0.08, 0.35, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	m = Translate(0.38, -0.07, -0.1) * Scale(0.15, 0.1, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void thanxe_manh()
{
	// phần thân dưới chỗ đứng ngắm pháo 
	paintColor(-50, -50, 0);
	m = Translate(0.66, -0.09, 0) * Scale(0.3, 0.14, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//bục đứng ngắm pháo  
	paintColor(102, 102, 0);
	m = Translate(0.72, 0.025, 0) * Scale(0.14, 0.05, 0.28);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//banh xe du phong
	model2_manh = Translate(0.55, 0.16, 0) * RotateY(90);
	banhxe_manh();

	//mat than xe 
	paintColor(0, 153, 0);
	m = Translate(1.18, 0.01, 0) * Scale(0.6, 0.02, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//phan cheo cua mat than
	paintColor(-50, -50, 0);
	m = Translate(0.84, 0, 0) * RotateZ(20) * Scale(0.1, 0.02, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// đuôi xe thanh sau xe 
	m = Translate(1.48, 0.09, 0) * Scale(0.02, 0.18, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//thanh dọc thùng xe trái
	paintColor(-50, -50, 0);
	m = Translate(1.18, 0.09, 0.34) * Scale(0.6, 0.18, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//thanh dọc thùng xe phải
	paintColor(-50, -50, 0);
	m = Translate(1.18, 0.09, -0.34) * Scale(0.6, 0.18, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//phần bục để pháo 
	paintColor(0, 0, 0);
	m = Translate(1.12, 0.05, 0) * Scale(0.3, 0.1, 0.38);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);



}

void bobanhxe_manh()
{
	model2_manh = Translate(0, -0.2, 0.25);
	banhxe_manh();

	model2_manh = Translate(0, -0.2, -0.25);
	banhxe_manh();

	model2_manh = Translate(1.35, -0.2, 0.25);
	banhxe_manh();

	model2_manh = Translate(1, -0.2, 0.25);
	banhxe_manh();

	model2_manh = Translate(1.35, -0.2, -0.25);
	banhxe_manh();

	model2_manh = Translate(1, -0.2, -0.25);
	banhxe_manh();
	//truc banh xe

	paintColor(-150, -150, -150);
	m = Translate(0, -0.2, 0) * Scale(0.02, 0.02, 0.48);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	m = Translate(1.35, -0.2, 0) * Scale(0.02, 0.02, 0.48);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	m = Translate(1, -0.2, 0) * Scale(0.02, 0.02, 0.48);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void dephao_manh()
{
	//trục đế 
	paintColor(0, 150, 0);
	m = Translate(1.12, 0.13, 0) * Scale(0.1, 0.16, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//mặt ngang đế 
	paintColor(30, 150, 0);
	m = Translate(1.12, 0.22, 0) * Scale(0.12, 0.02, 0.14);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thanh dọc trái
	paintColor(0, 150, 0);
	m = Translate(1.12, 0.28, 0.06) * Scale(0.12, 0.12, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//thanh dọc phải
	paintColor(0, 150, 0);
	m = Translate(1.12, 0.28, -0.06) * Scale(0.12, 0.12, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//thanh doc trước pháo
	paintColor(0, 100, 0);
	m = Translate(1.17, 0.26, 0) * Scale(0.02, 0.06, 0.12);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void nongphao_manh1()
{
	for (float i = 0; i < 360; i++)
	{
		paintColor(55, 33, 0);
		paintColor(0, 51, 51);
		mat4 m = RotateX(i) * Scale(0.7, 0.05, 0.05);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * model2_manh * m);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void nongphao_manh2()
{
	for (float i = 0; i < 360; i++)
	{
		paintColor(55, 33, 0);
		paintColor(0, 51, 51);
		mat4 m = Translate(0.4, 0, 0) * RotateX(i) * Scale(0.13, 0.07, 0.07);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * model2_manh * m);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void nongphao_manh()
{
	paintColor(0, 51, 51);
	mat4 m = Translate(-0.4, 0, 0) * Scale(0.12, 0.12, 0.12);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * model2_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	nongphao_manh1();
	nongphao_manh2();
}
GLfloat cdmanh = 0, bbmanh = 0;
void Dan() {
	paintColor(200, -115, -136);
	mat4 m = Scale(0.04, 0.04, 0.04) * Translate(cdmanh, 0, 0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model1_manh * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void banphao(void)
{
	cdmanh += 2;
	glutPostRedisplay();
}

void phao()
{
	//do đế pháo dịch chuyển bên trên là translate x = 1.12 nên 
	dephao_manh();
	// nòng pháo là con của đế pháo 
	model2_manh = Translate(1.24, 0.35, 0) * RotateZ(20);
	model1_manh = model1_manh * Translate(1.12, 0.35, 0) * RotateZ(thetamanh[9]) * Translate(-1.12, -0.35, 0);
	nongphao_manh();
	model1_manh = model1_manh * Translate(1.6, 0.49, 0) * RotateZ(20);
	Dan();
}
void xe_manh()
{
	model1_manh = Translate(3.8, -0.06, 0.6) * RotateY(-90);
	model1_manh = model1_manh *  RotateY(thetamanh[1]) * Scale(1.3, 1.3, 1.3) * Translate(pull, 0, 0);
	dauxe_manh();
	thanxe_manh();
	mat4 modelnew = model1_manh;
	model1_manh = model1_manh * Translate(0.2, 0, 0.23) * RotateY(thetamanh[5]) * Translate(-0.2, 0, 0);
	canhcua_manh();
	model1_manh = modelnew;
	model1_manh = model1_manh * Translate(0.2, 0, -0.23) * RotateY(thetamanh[6]) * Translate(-0.2, 0, 0);
	canhcua_manh();
	model1_manh = modelnew;
	model2_manh = RotateZ(thetamanh[3]);
	bobanhxe_manh();
	model1_manh = model1_manh * Translate(1.12, 0, 0) * RotateY(thetamanh[7]) * Translate(-1.12, 0, 0);;
	phao();
}
//NGUOI CUA DAO LINH 
void Cube(GLfloat x, GLfloat y, GLfloat z, GLfloat a, GLfloat b, GLfloat c) {
	mat4 instance = Translate(a, b, c) * Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_nguoi * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void than() {
	paintColor(100, 100, 0);
	Cube(0.2, 0.4, 0.1, 0, 0, 0);
	Cube(0.05, 0.05, 0.1, 0.125, 0.175, 0);
	Cube(0.05, 0.05, 0.1, -0.125, 0.175, 0);
}

void khuuTay1() {
	paintColor(100, 100, 0);
	mat4 instance = Scale(0.1, 0.2, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_nguoi * instance_tay * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void khuuTay2() {
	paintColor(100, 100, 0);
	mat4 instance = Scale(0.1, 0.2, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_nguoi * instance_tay * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void banTay() {
	paintColor(240, 230, 140);
	mat4 instance = Scale(0.05, 0.1, 0.05);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_nguoi * instance_tay * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
GLfloat tay[] = { 0,0,0,0 };
//void tay_trai() {
//	instance_tay = Translate(-0.2, 0.2, 0) * RotateX(beta[0]) * RotateZ(beta[1]) * RotateX(beta[5]) * Translate(0.0, -0.1, 0);
//	khuuTay1();
//	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateZ(beta[2]) * RotateX(beta[6]) * RotateY(beta[7])* Translate(0, -0.1, 0);
//	khuuTay2();
//	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateY(beta[3]) *  Translate(0, -0.05, 0);
//	banTay();
//}
//GLfloat phai[] = { 0,15,-40,-15 , 15,0,0 };
//void tay_phai() {
//	instance_tay = Translate(0.2, 0.2, 0) * RotateZ(phai[0]) *RotateX(phai[4]) * Translate(0.0, -0.1, 0);
//	khuuTay1();
//	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateX(phai[1]) * RotateZ(phai[2]) * *RotateY(phai[6]) * Translate(0, -0.1, 0);
//	khuuTay2();
//	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateX(phai[3]) * RotateX(phai[5]) * RotateY(beta[3]) * Translate(0, -0.05, 0);
//	banTay();
//}

void tay_trai() {
	instance_tay = Translate(-0.2, 0.2, 0) * RotateZ(beta[1]) * RotateX(beta[2]) * Translate(0.0, -0.1, 0);
	khuuTay1();
	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateZ(beta[5]) * RotateX(beta[6]) * RotateY(beta[7]) * Translate(0, -0.1, 0);
	khuuTay2();
	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateY(beta[3]) * Translate(0, -0.05, 0);
	banTay();
}
GLfloat phai[] = { 0,15,-40,-15 , 15,0,0,0 };
void tay_phai() {
	instance_tay = Translate(0.2, 0.2, 0) * RotateZ(phai[0]) * RotateX(phai[1]) * *RotateY(phai[7]) * Translate(0.0, -0.1, 0);
	khuuTay1();
	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateX(phai[4]) * RotateZ(phai[2]) * *RotateY(phai[6]) * Translate(0, -0.1, 0);
	khuuTay2();
	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateX(phai[3]) * RotateZ(phai[5]) * RotateY(beta[3]) * Translate(0, -0.05, 0);
	banTay();
}

void banchan(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 instance = Translate(x, y, z) * Scale(0.05, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_nguoi * instance_tay * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Váº½ cÃ¡c tam giÃ¡c*/
}

void banchan2(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 instance = Translate(x, y, z) * Scale(0.075, 0.05, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_nguoi * instance_tay * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Váº½ cÃ¡c tam giÃ¡c*/
}

void banChan3() {
	paintColor(100, 0, 0);
	banchan(0, 0, 0);
	paintColor(100, 0, 0);
	banchan2(0, -0.075, 0);
}


GLfloat chan[] = { 0,0,0,0,0,0,0 };
GLfloat chan1[] = { 0,0,0,0,0,0,0 };
void chanTrai() {
	instance_tay = Translate(-0.05, -0.2, 0) * RotateX(chan[1]) * RotateZ(chan[2]) * Translate(0.0, -0.1, 0);
	khuuTay1();
	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateX(chan1[1]) * Translate(0, -0.05, 0);
	banChan3();
}
void chanPhai() {
	instance_tay = Translate(0.05, -0.2, 0) * RotateX(chan[0]) * RotateZ(chan1[2]) * Translate(0.0, -0.1, 0);
	khuuTay1();
	instance_tay = instance_tay * Translate(0, -0.1, 0) * RotateX(chan1[0]) * Translate(0, -0.05, 0);
	banChan3();
}

mat4 dau_matrix;
GLfloat dx, dy, dz;

void dau()
{
	for (int i = 0; i < 360; i += 40)
	{
		paintColor(0, 0, 0);
		mat4 instance = RotateX(i) * RotateY(i) * RotateZ(i) * Scale(0.15, 0.15, 0.15);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_nguoi * dau_matrix * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Váº½ cÃ¡c tam giÃ¡c*/
	}
}

void coo()
{
	paintColor(240, 230, 140);
	mat4 instance = Scale(0.05, 0.1, 0.05);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_nguoi * dau_matrix * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Váº½ cÃ¡c tam giÃ¡c*/
}

void dau_nguoi()
{
	dau_matrix = Translate(0, 0.225, 0);
	coo();
	dau_matrix = dau_matrix * Translate(0, 0.1, 0) * RotateX(dx) * RotateY(dy) * RotateZ(dz) * Translate(0, 0.05, 0);
	dau();
}

GLfloat xx, yy, zz;
GLfloat nguoi[] = { 0,0,0 };
void full_nguoi() {
	//model_nguoi = model_nguoi * Translate(xx, yy, zz);
	than();
	tay_trai();
	tay_phai();
	chanPhai();
	chanTrai();
	dau_nguoi();
}


int t = 0, T = 2, l = 0, r = 0;
void buoctruoc()
{
	if (t <= 2)
	{
		beta[0] += 10;
		chan[1] += 10; chan1[1] -= 10; chan[0] -= 10; chan1[0] += 10; zz -= 0.01;
		glutPostRedisplay();
		zz -= 0.05;
		t++;
	}
	if (t > T)
	{
		beta[0] -= 10;
		chan[1] -= 10; chan1[1] += 10; chan[0] += 10; chan1[0] -= 10; zz += 0.01;
		zz -= 0.05;
		glutPostRedisplay();
		T--;
	}
	if (T == -3)
	{
		beta[0] = 0;
		chan[1] = 0; chan1[1] = 0; chan[0] = 0; chan1[0] = 0;
		glutPostRedisplay();
		t = 0; T = 2;
	}
}

void buocsau()
{
	if (t <= 2)
	{
		beta[0] += 10;
		chan[1] += 10; chan1[1] -= 10; chan[0] -= 10; chan1[0] += 10; zz -= 0.01;
		zz += 0.05;
		glutPostRedisplay();
		t++;
	}
	if (t > T)
	{
		beta[0] -= 10;
		chan[1] -= 10; chan1[1] += 10; chan[0] += 10; chan1[0] -= 10; zz += 0.01;
		zz += 0.05;
		glutPostRedisplay();
		T--;
	}
	if (T == -3)
	{
		beta[0] = 0;
		chan[1] = 0; chan1[1] = 0; chan[0] = 0; chan1[0] = 0;
		glutPostRedisplay();
		t = 0; T = 2;
	}
}
void buoctrai() {
	if (l == r) {
		chan[2] -= 10;
		glutPostRedisplay();
		l++;
	}
	else {
		chan[2] += 10;
		xx -= 0.05;
		glutPostRedisplay();
		l--;
	}
}
void buocphai() {
	if (l == r) {
		chan1[2] += 10;
		glutPostRedisplay();
		r++;
	}
	else {
		chan1[2] -= 10;
		xx += 0.05;
		glutPostRedisplay();
		r--;
	}
}

void CacBoPhanSung(float Tx, float Ty, float Tz, float Sx, float Sy, float Sz, float Rz)
{
	mat4 tt = Translate(Tx, Ty, Tz) * RotateZ(Rz) * Scale(Sx, Sy, Sz);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_linh * model_model * tt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void CacBoPhanSung2(float Tx, float Ty, float Tz, float Sx, float Sy, float Sz, float Rz)
{
	mat4 tt = Translate(Tx, Ty, Tz) * RotateZ(Rz) * Translate(-Sx / 2, 0, 0) * Scale(Sx, Sy, Sz);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_linh * model_model * tt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void CacBoPhanSungTru(float h, float r, float x1, float y1, float z1, float start, float end, float q)
{
	mat4 t1 = Translate(x1, y1, z1) * RotateX(q);
	mat4 ctm;
	ctm = Translate(0, r / 2, 0) * Scale(h, r, 0.01);
	for (int i = 0; i <= end; i++)
	{
		paintColor(50, 45, 36);
		ctm = RotateX(i) * ctm;
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_linh * t1 * ctm);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

}
GLfloat theta[] = { 0,0,0 };
void Nong_sung(float h, float r, float x1, float y1, float z1, float start, float end, float q)
{
	mat4 t1 = Translate(x1, y1, z1) * RotateX(q);
	mat4 ctm;
	ctm = Translate(0, r / 2, 0) * Scale(h, r, 0.01);
	for (int i = 0; i <= end; i++)
	{
		paintColor(174, 110, 75);
		ctm = RotateX(i) * ctm;
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_linh * t1 * ctm);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void Nong_sung_nguoc(float h, float r, float x1, float y1, float z1, float start, float end, float q)
{
	/*
	h là Chiều Dài hình trụ theo trục Ox
	r là Bán Kính hình trụ theo trục Oy
	t1 là mat4 để dịch chuyển, ghép thành main
	*/
	mat4 t1 = Translate(x1, y1, z1) * RotateX(q);
	mat4 ctm;
	ctm = Translate(0, r / 2, 0) * Scale(h, r, 0.01);
	for (int i = 0; i >= end; i--)
	{
		paintColor(174, 110, 75);
		ctm = RotateX(i) * ctm;
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_linh * t1 * ctm);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void thanSung() {
	paintColor(50, 45, 36);
	CacBoPhanSung(0, 0, 0, 0.35, 0.1, 0.04, 0);
	paintColor(174, 110, 75);
	CacBoPhanSung2(-0.17, 0.0, 0, 0.25, 0.05, 0.03, 15);
	paintColor(174, 110, 75);
	CacBoPhanSung2(-0.125, -0.085, 0, 0.03, 0.07, 0.03, -15);
	paintColor(50, 45, 36);
	CacBoPhanSung2(0.12, -0.1, 0, 0.05, 0.135, 0.035, 15);
	paintColor(50, 45, 36);
	CacBoPhanSung(0.16, 0.06, 0, 0.08, 0.07, 0.04, 0);
	paintColor(174, 110, 75);
	Nong_sung(0.03, 0.02, 0.2, 0.058, 0, 0, 360, 0);
	//khung dung cof
	paintColor(50, 45, 36);
	CacBoPhanSung(-0.1, -0.055, 0, 0.005, 0.1, 0.025, 0);
	paintColor(50, 45, 36);
	CacBoPhanSung(-0.05, -0.1, 0, 0.1, 0.005, 0.025, 0);
	paintColor(50, 45, 36);
	CacBoPhanSung(0, -0.055, 0, 0.01, 0.1, 0.025, 0);
	CacBoPhanSung(-0.06, -0.065, 0, 0.015, 0.035, 0.025, 15);
}
float bb = 15;
void co() {
	CacBoPhanSung2(-0.06, -0.06, 0, 0.015, 0.03, 0.025, bb);
}

void oplot() {
	paintColor(174, 110, 75);
	CacBoPhanSung(0.29, 0.054, 0, 0.15, 0.07, 0.038, 0);
	Nong_sung(0.15, 0.022, 0.29, 0.08, 0, 0, 13, 0);
	Nong_sung_nguoc(0.15, 0.022, 0.29, 0.08, 0, 0, -13, 0);
	paintColor(174, 110, 75);
	CacBoPhanSung(0.27, -0.015, 0, 0.2, 0.06, 0.038, 0);
	Nong_sung(0.2, 0.022, 0.27, -0.04, 0, 0, 13, 180);
	Nong_sung_nguoc(0.2, 0.022, 0.27, -0.04, 0, 0, -13, 180);
}
void dauSung() {
	CacBoPhanSungTru(0.35, 0.018, 0.52, -0.025, 0, 0, 360, 0);
	CacBoPhanSungTru(0.14, 0.02, 0.43, 0.06, 0, 0, 360, 0);
	CacBoPhanSung(0.5, 0.02, 0, 0.02, 0.07, 0.02, 30.0);
	//CacBoPhanSung(0.67, 0.02, 0, 0.02, 0.07, 0.05, 0);
}
GLfloat cd = 0;
void Dan_linh() {
	paintColor(50, 45, 36);
	mat4 ctm = Scale(0.03, 0.03, 0.03) * Translate(cd, 2.8, 0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tong * model_linh * ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

void banphao_linh(void)
{
	cd += 0.6;
	glutPostRedisplay();
}

void full() {
	thanSung();
	oplot();
	dauSung();
	co();
}
GLfloat sung[] = { 10,30,73 };
GLfloat s1, s2, s3;
void sung_full() {
	model_linh = Translate(s1, s2, s3) * Translate(0.05, -0.2, -0.15) * RotateX(sung[0]) * RotateY(sung[1]) * Translate(0, 0.25, 0) * RotateZ(sung[2]) * Scale(0.65, 0.65, 1);
	full();
	model_linh = model_linh * Translate(0.3, -0.019, 0);
	Dan_linh();
}
GLfloat rbx, rby, rbz;
void tong(GLfloat x, GLfloat y, GLfloat z, GLfloat a, GLfloat b, GLfloat c) {
	model_tong = RotateY(nguoi[0]) * Translate(x, y, z) * Scale(a, b, c);
	model_tong = model_tong * Translate(xx, yy, zz);
	full_nguoi();
	model_tong = model_tong * Translate(0, 0, 0);
	sung_full();
}
void xephanglinh()
{
	tong(0, -0.1, 1.6, 0.8, 0.8, 0.8);
	tong(0.9, -0.1, 1.6, 0.8, 0.8, 0.8);
	tong(-0.9, -0.1, 1.6, 0.8, 0.8, 0.8);
	tong(0.45, -0.1, 0.8, 0.8, 0.8, 0.8);
	tong(-0.45, -0.1, 0.8, 0.8, 0.8, 0.8);
}

//XE CỦA TÂM 
mat4 mt;
void vongtron_t()
{
	for (float i = 0; i < 360; i++)
	{
		mt = RotateZ(i) * Translate(0, 0, 0) * Scale(0.22, 0.2f, 0.11f);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * model3t * mt);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

}
void taycamphao_t()
{
	for (float i = 0; i < 360; i++)
	{
		mt = RotateZ(i) * Scale(0.05, 0.015, 0.6);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * model4t * mt);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void banhxe_t()
{
	for (float i = 0; i < 360; i = i + 10)
	{
		paintColor(-50, -50, -50);
		mt = RotateZ(i) * Translate(0.12, 0, 0) * Scale(0.09, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * model2t * mt);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}
	for (float i = 0; i < 360; i++)
	{
		paintColor(102, 204, 0);
		mt = RotateZ(i) * Translate(0.09, 0, 0) * Scale(0.02, 0.05, 0.11);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * model2t * mt);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	for (float i = 0; i < 360; i++)
	{
		paintColor(-50, -50, -50);
		mt = RotateZ(i) * Translate(0.05, 0, 0) * Scale(0.07, 0.05, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * model2t * mt);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void canhcua_t()
{
	//canh cua ben phai
	//tao phan duoi
	paintColor(0, 51, 0);
	mt = Translate(0.34, 0.02, 0) * Scale(0.3, 0.3, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tao phan duoi
	mt = Translate(0.205, 0.25, 0) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tao khung phan tren
	mt = Translate(0.494, 0.25, 0) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tao thanh tren; rotateZ ???
	mt = Translate(0.35, 0.35, 0) * RotateZ(4) * Scale(0.29, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void dauxe_t()
{
	//truc xe
	paintColor(0, 0, 0);
	mt = Translate(1.12, -0.04, 0) * Scale(0.7, 0.1, 0.38);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	paintColor(0, 150, 0);
	mt = Translate(0, 0, 0) * Scale(0.4, 0.3, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	paintColor(0, 96, 0);
	mt = Translate(0.012, 0.12, 0) * RotateZ(3) * Scale(0.42, 0.1, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//2 thanh gan banh xe
	mt = Translate(-0.085, 0, -0.25) * Scale(0.25, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(-0.085, 0, 0.25) * Scale(0.25, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//2 thanh cheo gan banh xe
	paintColor(0, 153, 0);
	mt = Translate(0.115, -0.07, -0.25) * RotateZ(-40) * Scale(0.22, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.115, -0.07, 0.25) * RotateZ(-40) * Scale(0.22, 0.02, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//den xe
	paintColor(255, 255, 153);
	mt = Translate(-0.16, -0.05, -0.24) * Scale(0.05, 0.1, 0.08);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(-0.16, -0.05, 0.24) * Scale(0.05, 0.1, 0.08);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//thanh dau xe
	paintColor(-50, -50, -50);
	mt = Translate(-0.225, -0.125, 0) * Scale(0.05, 0.05, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(-0.225, -0.175, 0) * Scale(0.05, 0.05, 0.35);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//dau xe (may thanh o giua)
	paintColor(-10, -10, -10);
	//thanh tren dinh
	mt = Translate(-0.21, 0.11, 0) * Scale(0.02, 0.02, 0.25);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//2 thanh 2 canh
	mt = Translate(-0.21, 0, -0.115) * Scale(0.02, 0.2, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(-0.21, 0, 0.115) * Scale(0.02, 0.2, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//2 thanh 2 canh
	mt = Translate(-0.21, 0, 0) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	mt = Translate(-0.21, 0, 0.08) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	mt = Translate(-0.21, 0, -0.08) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	mt = Translate(-0.21, 0, 0.05) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	mt = Translate(-0.21, 0, -0.05) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//4 thanh cua
	paintColor(0, 153, 0);
	mt = Translate(0.205, 0.25, -0.23) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.205, 0.25, 0.23) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.494, 0.25, -0.23) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.494, 0.25, 0.23) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//noc xe kt
	paintColor(0, 153, 0);
	mt = Translate(0.35, 0.35, 0) * RotateZ(5) * Scale(0.3, 0.02, 0.46);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//cho len xe
	paintColor(0, 153, 0);

	mt = Translate(0.35, -0.15, 0) * Scale(0.3, 0.02, 0.63);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mieng lot chan
	paintColor(255, 153, 0);
	mt = Translate(0.35, -0.13, 0) * Scale(0.3, 0.04, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mieng dang sau than xe
	paintColor(0, 153, 0);
	mt = Translate(0.5, 0.05, 0) * Scale(0.02, 0.35, 0.47);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//Ghe ngoi
	paintColor(0, 76, 153);
	mt = Translate(0.45, 0.05, 0.1) * Scale(0.08, 0.35, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.38, -0.05, 0.1) * Scale(0.15, 0.1, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.45, 0.05, -0.1) * Scale(0.08, 0.35, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.38, -0.07, -0.1) * Scale(0.15, 0.1, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanxe_t()
{
	paintColor(0, 204, 102);
	// phan bá»‡ tren
	mt = Translate(0.66, -0.09, 0) * Scale(0.3, 0.14, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//hop sua chua
	paintColor(204, 102, 0);
	mt = Translate(0.56, 0.15, 0.02) * Scale(0.11, 0.1, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//cho dung
	paintColor(0, 204, 102);
	mt = Translate(0.74, -0.015, 0) * Scale(0.22, 0.01, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// khung o giua
	paintColor(102, 102, 0);
	mt = Translate(0.74, 0.05, 0) * Scale(0.22, 0.12, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//2 tay nam
		//2 tay ngang
		//ben trai
	mt = Translate(0.64, 0.15, 0.1) * Scale(0.01, 0.08, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.7, 0.19, 0.1) * Scale(0.13, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.7, 0.15, 0.1) * Scale(0.13, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.76, 0.15, 0.1) * Scale(0.01, 0.08, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//2 thanh doc tay cam
	mt = Translate(0.68, 0.17, 0.1) * Scale(0.01, 0.04, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.72, 0.17, 0.1) * Scale(0.01, 0.04, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//ben phai
	mt = Translate(0.64, 0.15, -0.1) * Scale(0.01, 0.08, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.7, 0.19, -0.1) * Scale(0.13, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.7, 0.15, -0.1) * Scale(0.13, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.76, 0.15, -0.1) * Scale(0.01, 0.08, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//2 thanh doc tay cam
	mt = Translate(0.68, 0.17, -0.1) * Scale(0.01, 0.04, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(0.72, 0.17, -0.1) * Scale(0.01, 0.04, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mat than xe
	paintColor(0, 153, 0);
	mt = Translate(1.18, 0.01, 0) * Scale(0.6, 0.02, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//2 phan cheo cua mat than
	paintColor(0, 153, 0);
	mt = Translate(0.84, 0, 0) * RotateZ(20) * Scale(0.1, 0.02, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(1.52, -0.035, 0) * RotateZ(-40) * Scale(0.15, 0.02, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void nongphao_t()
{
	for (float i = 0; i < 360; i++)
	{
		paintColor(0, 51, 51);
		mat4 mt = RotateX(i) * Translate(0, 0, 0.025) * Scale(0.7, 0.01, 0.01);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * model5t * model4t * model3t * mt);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void phao_t()
{
	model3t = Translate(0, 0, -0.05);
	nongphao_t();

	model3t = Translate(0, 0, -0.1);
	nongphao_t();
	model3t = {};
	nongphao_t();
	model3t = Translate(0, 0, 0.05);
	nongphao_t();

	model3t = Translate(0, 0, 0.1);
	nongphao_t();

	model3t = Translate(0, 0, 0.15);
	nongphao_t();
}
void nhieuphao()
{
	model4t = Translate(0, 0.025, 0);
	phao_t();

	model4t = Translate(0, 0.075, 0);
	phao_t();

	model4t = Translate(0, -0.025, 0);
	phao_t();

	model4t = Translate(0, -0.075, 0);
	phao_t();
}
void bedophao_t()
{
	paintColor(0, 150, 0);
	mt = Translate(1.33, 0.03, 0) * Scale(0.2, 0.028, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	model3t = Translate(1.33, 0.1, 0) * RotateX(90);
	vongtron_t();
}
void banhphao_t()
{
	mt = Translate(1.33, 0.18, 0) * Translate(0, 0.025, 0) * Scale(0.42, 0.05, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	model5t = Translate(1.32, 0.35, -0.02)  /*RotateZ(-30) */;
	nhieuphao();
	//tay cam thanh dieu khien
	model4t = Translate(1.56, 0.18, 0);
	taycamphao_t();
	mt = Translate(1.59, 0.18, 0.3) * Scale(0.12, 0.05, 0.05);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//khung om phao
	paintColor(0, 204, 0);
	mt = Translate(1.4, 0.33, 0.2) * Scale(0.41, 0.25, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(1.43, 0.46, 0) * Scale(0.4, 0.02, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mt = Translate(1.4, 0.33, -0.2) * Scale(0.41, 0.25, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

void bobanhxe_t()
{
	model2t = Translate(0, -0.2, 0.25);
	banhxe_t();

	model2t = Translate(0, -0.2, -0.25);
	banhxe_t();

	model2t = Translate(1.35, -0.2, 0.25);
	banhxe_t();

	model2t = Translate(1, -0.2, 0.25);
	banhxe_t();

	model2t = Translate(1.35, -0.2, -0.25);
	banhxe_t();

	model2t = Translate(1, -0.2, -0.25);
	banhxe_t();

	//truc banh xe
	paintColor(-150, -150, -150);
	mt = Translate(0, -0.2, 0) * Scale(0.02, 0.02, 0.48);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	mt = Translate(1.35, -0.2, 0) * Scale(0.02, 0.02, 0.48);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	mt = Translate(1, -0.2, 0) * Scale(0.02, 0.02, 0.48);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//
}

GLfloat v_t;

GLfloat banDan_T = 0;
GLfloat coDan_T = 0;
GLfloat pull_tam = 0;
void dan() {
	paintColor(96, 96, 69);
	mt = Translate(1.35, 0.33, -0.02) * Scale(0.6, 0.03, 0.03);;
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelt * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void modelphao_t()
{
	bedophao_t();
	mat4 temp = modelt;
	modelt = modelt * Translate(1.46, 0, 0) * RotateZ(tamta[8]) * Translate(-1.56, -0.05, 0);
	banhphao_t();
	//temp = model;
	modelt = modelt * Translate(banDan_T, 0, 0) * Scale(coDan_T, coDan_T, coDan_T);
	dan();
}

void xe_kt()
{
	modelt = RotateY(tamta[1]) * RotateY(-90) * Translate(1, -0.06, 1.7) * Scale(1.3, 1.3, 1.3);
	modelt = modelt * Translate(pull_tam, 0, 0);
	dauxe_t();
	mat4 temp = modelt;
	modelt = modelt * Translate(0.2, 0, 0.23) * RotateY(tamta[5]) * Translate(-0.2, 0, 0);
	canhcua_t();
	modelt = temp;
	modelt = modelt * Translate(0.2, 0, -0.23) * RotateY(tamta[6]) * Translate(-0.2, 0, 0);
	canhcua_t();
	modelt = temp;
	thanxe_t();
	modelbanh_kt = RotateZ(tamta[3]);
	bobanhxe_t();
	temp = modelt;
	modelt = modelt * Translate(1.4, 0, 0) * RotateY(tamta[7]) * Translate(-1.4, 0, 0);
	modelphao_t();

}

void ban_T() {
	if (banDan_T == 0) {
		coDan_T = 1;
	}
	banDan_T -= v_t;
	if (banDan_T + v_t <= -3) {
		v_t = 0;
		banDan_T = 0;
		coDan_T = 0;
	}
	glutPostRedisplay();
}




// XE CỦA DUYÊN 



void den_D() {
	mat4 instance;
	//Đèn 1
	paintColor(255, 255, 255);//Màu trắng
	instance = Translate(0.91, -0.1, 0.4) * Scale(0.02, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//Thanh chắn
	paintColor(0, 0, 0);//Màu đen
	instance = Translate(0.025 + 0.9, 0, 1 / 2 - 0.1) * Scale(0.05, 0.01, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.025 + 0.9, -0.2, 1 / 2 - 0.1) * Scale(0.05, 0.01, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, 1 / 2 - 0.05) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, 1 / 2 - 0.075) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, 1 / 2 - 0.1) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, 1 / 2 - 0.125) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, 1 / 2 - 0.15) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.925, -0.1, 1 / 2 - 0.05) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//Đèn 2
	paintColor(255, 255, 255);//Màu trắng
	instance = Translate(00.91, -0.1, -0.4) * Scale(0.02, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//Thanh chắn
	paintColor(0, 0, 0);//Màu đen
	instance = Translate(0.025 + 0.9, 0, -1 / 2 + 0.1) * Scale(0.05, 0.01, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.025 + 0.9, -0.2, -1 / 2 + 0.1) * Scale(0.05, 0.01, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, -1 / 2 + 0.05) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, -1 / 2 + 0.075) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, -1 / 2 + 0.1) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, -1 / 2 + 0.125) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.95, -0.1, -1 / 2 + 0.15) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0.005 + 0.925, -0.1, -1 / 2 + 0.05) * Scale(0.01, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanxe_D()
{
	paintColor(0, 150, 0);
	mat4 instance = Translate(-0.9, 0, 0) * Scale(1.8, 0.4, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// mũi xe
	paintColor(0, 150, 0);
	instance = Translate(0.45, -0.1, 0) * Scale(0.9, 0.2, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thanh chéo
	paintColor(100, 150, 0);
	GLfloat goc = atan(0.2 / 0.9) * 180 / M_PI;
	instance = Translate(0.46, 0.1, 0) * RotateZ(-goc) * Scale(0.92, 0.01, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//chắn 2 mũi xe
	paintColor(100, 150, 0);
	instance = Translate(0.46, 0.1, 0) * Scale(0.92, 0.2, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	paintColor(100, 150, 0);
	instance = Translate(0.45, 0.1, 0) * RotateZ(-90) * RotateY(180) * Scale(0.2, 0.9, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	den_D();
}

void thanxeTren_D()
{

	// giữa
	paintColor(100, 150, 0);
	mat4 instance = Translate(-0.4, 0.4, 0) * Scale(0.8, 0.4, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// sau
	paintColor(100, 150, 0);
	instance = Translate(-0.95, 0.3, 0) * Scale(0.3, 0.2, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//chéo sau
	GLfloat goc = atan(0.2 / 0.3) * 180 / M_PI;
	instance = Translate(-0.95, 0.5, 0) * RotateZ(goc) * Scale(0.36, 0.01, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//chắn chéo 2 bên
	instance = Translate(-0.95, 0.5, 0) * RotateY(-180) * Scale(0.36, 0.2, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	instance = Translate(-0.95, 0.5, 0) * RotateY(360) * RotateZ(90) * Scale(0.2, 0.36, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void hinhtronDet(mat4 a_D)
{
	paintColor(100, 100, 0);
	for (float i = 0; i < 360; i++)
	{
		mat4 instance = RotateZ(i) * Scale(0.2, 0.4, 0.01);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * a_D * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void napxe_D()
{
	mat4 a = Translate(-0.4, 0.605, 0) * RotateX(90);
	hinhtronDet(a);
}

void hinhtronRong(mat4 a_D)
{
	paintColor(-50, -50, -50);
	for (float i = 0; i < 360; i++) {
		mat4 instance = RotateZ(i) * Translate(0.15, 0, 0) * Scale(0.1, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * a_D * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void thanxeDuoi_D()
{
	// thanh chắn cuối
	paintColor(200, 200, 0);
	mat4 instance = Translate(-1.65, -0.25, 0) * Scale(0.3, 0.1, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// thanh chắn 2
	instance = Translate(-1.15, -0.25, 0) * Scale(0.3, 0.1, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thanh chắn 3
	instance = Translate(-0.45, -0.25, 0) * Scale(0.1, 0.1, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thanh chắn 4
	instance = Translate(0.05, -0.25, 0) * Scale(0.1, 0.1, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thanh chắn 5
	instance = Translate(0.65, -0.25, 0) * Scale(0.1, 0.1, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// gầm xe
	paintColor(0, 150, 0);
	instance = Translate(-0.55, -0.35, 0) * Scale(2.5, 0.3, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void hinhtronDac(mat4 a_D)
{
	paintColor(255, 255, 255);
	for (float i = 0; i < 360; i++)
	{
		mat4 instance = RotateZ(i) * Scale(0.15, 0.15, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * a_D * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
mat4 a_D;
void banhxe_D()
{
	//tính từ trái -> phải
	a_D = Translate(-1.3, -0.5, 0.45);
	hinhtronRong(a_D);
	hinhtronDac(a_D);

	a_D = Translate(-1.3, -0.5, -0.45);
	hinhtronRong(a_D);
	hinhtronDac(a_D);

	a_D = Translate(-0.8, -0.5, 0.45);
	hinhtronRong(a_D);
	hinhtronDac(a_D);

	a_D = Translate(-0.8, -0.5, -0.45);
	hinhtronRong(a_D);
	hinhtronDac(a_D);

	a_D = Translate(-0.2, -0.5, 0.45);
	hinhtronRong(a_D);
	hinhtronDac(a_D);

	a_D = Translate(-0.2, -0.5, -0.45);
	hinhtronRong(a_D);
	hinhtronDac(a_D);

	a_D = Translate(0.3, -0.5, 0.45);
	hinhtronRong(a_D);
	hinhtronDac(a_D);

	a_D = Translate(0.3, -0.5, -0.45);
	hinhtronRong(a_D);
	hinhtronDac(a_D);
}

void nongphao_D()
{
	paintColor(100, 150, 0);
	GLfloat goc = 0;
	for (int i = 0; i < 360; i++)
	{
		goc += 0.5;
		mat4 instance = Translate(0.5, 0.4, 0) * RotateX(goc) * Scale(1, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	// đầu nòng pháo
	paintColor(-50, -50, -50);
	mat4 instance = Translate(1.15, 0.385, 0) * Scale(0.3, 0.15, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

GLfloat b_D = 0.09, s_D = 0;
void dan_D()
{
	mat4 instance = Scale(0.09, 0.09, 0.09) * Translate(s_D, 0, 0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_D * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void bandan_D()
{
	s_D += 1;
	glutPostRedisplay();
}

GLfloat theta_D[] = { 0, 0, 0, 0 };
GLfloat dr_D = 0;
GLfloat xx_D = 0, yy_D = 0, zz_D = 0;

void phao_D()
{
	
	thanxeTren_D();
	mat4 tmp = ctm_D;
	ctm_D *= Translate(0, 0.4, 0) * RotateZ(theta_D[2]) * Translate(0, -0.4, 0);
	nongphao_D();

	ctm_D *= Translate(1.045, 0.345, 0);
	dan_D();
	ctm_D = tmp;
}

void thanxetang_D()
{
	thanxe_D();
	thanxeDuoi_D();

	a_D *= RotateZ(theta_D[3]);
	banhxe_D();

}

void xe_D()
{
	ctm_D = RotateY(90) * Translate(-1.4, 0.02 , 1.7) * Scale(0.8, 0.8, 0.8);
	ctm_D =ctm_D *  RotateY(dr_D) * Translate(xx_D, 0, 0);
	thanxetang_D();

	mat4 tmp = ctm_D;
	ctm_D *= Translate(-0.4, 0, 0) * RotateY(theta_D[0]) * Translate(0.4, 0, 0);
	phao_D();

	ctm_D *= Translate(-0.2, 0.605, 0) * RotateZ(theta_D[1]) * Translate(0.2, -0.605, 0);
	napxe_D();
	ctm_D = tmp;
}


// XE CỦA HOÀI 



void cua_H(GLfloat x, GLfloat y, GLfloat z)
{
	paintColor(34, 139, 34);
	// thanh tren
	instance_H = Translate(x, y, z) * Translate(-0.19, 0.45, 0) * Scale(0.23, 0.07, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/

	// thanh duoi
	instance_H = Translate(x, y, z) * Translate(-0.275, 0.125, 0) * Scale(0.4, 0.25, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/

	// thanh doc
	instance_H = Translate(x, y, z) * Translate(-0.04, 0.24, 0) * Scale(0.07, 0.48, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/

	// thanh cheo
	instance_H = Translate(x, y, z) * RotateZ(-38) * Translate(-0.515, 0.038, 0) * Scale(0.05, 0.3, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void dauxe_H()
{
	cua_H(0, 0, 0.2);
	cua_H(0, 0, -0.2);

	// kinh
	paintColor(175, 238, 238);

	instance_H = RotateZ(-38) * Translate(-0.515, 0.038, 0) * Scale(0.05, 0.3, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/

	// nap
	paintColor(34, 139, 34);
	instance_H = Translate(-0.15, 0.465, 0) * Scale(0.295, 0.04, 0.38);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/

	// thanh truoc
	instance_H = Translate(-0.48, 0.125, 0) * Scale(0.04, 0.25, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void khungdoc_H(GLfloat x, GLfloat y, GLfloat z)
{
	paintColor(107, 142, 35);
	instance_H = Translate(x, y, z) * Scale(0.02, 0.5, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}


void khungngang_H(GLfloat x, GLfloat y, GLfloat z)
{
	paintColor(107, 142, 35);
	instance_H = Translate(x, y, z) * Scale(1, 0.5, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void ghe_H(GLfloat x, GLfloat y, GLfloat z)
{
	paintColor(143, 188, 143);
	instance_H = Translate(x, y, z) * Scale(0.95, 0.02, 0.12);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void napthan_H(GLfloat x, GLfloat y, GLfloat z)
{
	paintColor(107, 142, 35);
	instance_H = Translate(x, y, z) * Scale(1, 0.02, 0.42);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void cuathungxe_H()
{
	paintColor(107, 142, 35);
	instance_H = Translate(0.01, 0.1, 0) * Scale(0.02, 0.2, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * ctm_cua_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thungxe_H()
{

	khungdoc_H(0.01, 0.25, 0);
	khungngang_H(0.5, 0.25, 0.2);
	khungngang_H(0.5, 0.25, -0.2);
	ghe_H(0.5, 0.1, 0.125);
	ghe_H(0.5, 0.1, -0.125);
	napthan_H(0.5, 0.5, 0);

}

void day_H(GLfloat x, GLfloat y, GLfloat z)
{
	paintColor(0, 100, 0);
	instance_H = Translate(x, y, z) * Scale(1.5, 0.05, 0.42);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanxe_H()
{
	dauxe_H();
	thungxe_H();
	day_H(0.25, -0.025, 0);
}

void trucbanh_H(GLfloat x, GLfloat y, GLfloat z)
{
	instance_H = Translate(x, y, z) * Scale(0.1, 0.1, 0.42);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * ctm_banh_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanh_H(GLfloat x, GLfloat y, GLfloat z, GLfloat goc)
{
	paintColor(95, 185, 160);
	instance_H = Translate(x, y, z) * RotateZ(goc) * Scale(0.02, 0.28, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * ctm_banh_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void hinhtru_H(GLfloat x, GLfloat y, GLfloat z)
{
	paintColor(47, 79, 79);

	//vong tron
	instance_H = identity();
	for (int i = 0; i < 360; i++)
	{
		instance_H = RotateZ(i) * Translate(x, y, z) * Scale(0.2, 0.2, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * ctm_banh_H * instance_H);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void banh_H(GLfloat x, GLfloat y, GLfloat z)
{

	hinhtru_H(0, 0, 0.21);
	thanh_H(0, 0, 0.26, 0);
	thanh_H(0, 0, 0.26, 45);
	thanh_H(0, 0, 0.26, 90);
	thanh_H(0, 0, 0.26, 135);

	hinhtru_H(0, 0, -0.21);
	thanh_H(0, 0, -0.26, 0);
	thanh_H(0, 0, -0.26, 45);
	thanh_H(0, 0, -0.26, 90);
	thanh_H(0, 0, -0.26, 135);

	ctm_banh_H = Translate(x, y, z);
}

void banhxe_H(GLfloat x, GLfloat y, GLfloat z)
{
	trucbanh_H(0, 0, 0);
	banh_H(0, -0.1, 0);

	ctm_banh_H *= Translate(x, y, z);
}

void kinh_H(GLfloat x, GLfloat y, GLfloat z)
{
	paintColor(175, 238, 238);
	//chéo
	instance_H = Translate(x, y, z) * Scale(0.25, 0.2, 0.01) * Shear(0.65, 0, 0, 0, 0, 0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * ctm_kinh_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance_H = Translate(x, y, z) * Translate(0.1, 0, 0) * Scale(0.2, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, ctm_H * ctm_kinh_H * instance_H);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

void xetaiqs_H()
{
	ctm_H = RotateY(-90) * Translate(1.5, -0.222, 3.2) * Scale(1.3, 1.3, 1.3);
	ctm_H = ctm_H * Translate(xx_H, 0, 0) * RotateY(theta_h[0]);
	thanxe_H();
	ctm_cua_H = Translate(0.98, 0, 0) * RotateZ(theta_h[1]);
	cuathungxe_H();
	ctm_kinh_H = Translate(0, yy_h, 0);
	kinh_H(-0.25, 0.32, -0.2);
	ctm_kinh_H = Translate(0, yy_h, 0);
	kinh_H(-0.25, 0.32, 0.2);
	ctm_banh_H *= RotateZ(theta_h[2]);
	banhxe_H(-0.2, 0, 0);
	ctm_banh_H *= RotateZ(theta_h[2]);
	banhxe_H(0.8, 0, 0);
}

mat4 model_bg;
void background() {
	paintColor(137, 110, 110);
	mat4 instance = Translate(0, -0.6, 0) * Scale(20, 0.1, 20);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bg * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	paintColor(-60, 255, -55);
	instance = Translate(0, -0.5, -3.8) * Scale(18, 0.1, 1.3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bg * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
GLfloat a =0 , b = 3 , c = 3;
GLfloat zNear = 0.7, zFar = 5;
void reshape(int width, int height)
{
	vec4 eye(a, b, c, 1);
	vec4 at(0, 0, 0, 1);
	vec4 up(0, 1, 0, 1);

	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	projection = Frustum(-1, 1, -1, 1, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	glViewport(0, 0, width, height);
}
//bia
mat4 model2_bia;
void bia()
{
	
	/*mat4 instance = Translate(0, -0.8, 0) * Scale(0.1, 0.8, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model2_bia * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);*/

	paintColor(255, 255, 0);
	mat4 instance = Translate(0, -1.3, 0) * Scale(0.04, 2, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bia  * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void bia2() {

	/*mat4 instance = Translate(0, 0, 0.02) * Scale(1, 1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);*/
	mat4 instance;
	for (float i = 0; i < 360; i++)
	{
		paintColor(255, 255, 0);
		instance = RotateZ(i) * Translate(0, 0, 0) * Scale(0.1, 0.1, 0.11f);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bia * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	for (float i = 0; i < 360; i++)
	{
		paintColor(255, 0, 0);
		instance = RotateZ(i) * Translate(0.2, 0, 0) * Scale(0.1, 0.1, 0.11f);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bia * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	for (float i = 0; i < 360; i++)
	{
		paintColor(255, 0, 0);
		instance = RotateZ(i) * Translate(0.4, 0, 0) * Scale(0.1, 0.1, 0.11f);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bia * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	paintColor(-50, -50, -50);
	instance = Translate(0, 0, -0.02) * Scale(1, 1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bia * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void bia3() {
	model_bia = model_bia * Scale(0.8, 0.8, 0.8);
	bia();
	bia2();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4 eye(a, b, c, 1);
	vec4 at(0, 0, 0, 1);
	vec4 up(0, 1, 0, 1);

	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	projection = Frustum(-1, 1, -1, 1, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	//glViewport(0, 0, width, height);
	const vec3 viewer_pos(a, b, 3.0);  /*Trùng với eye của camera*/
	background();
	
	xe_manh();
	xephanglinh();
	xe_kt();
	xe_D();
	xetaiqs_H();
	model_bia = Translate(4, 1.2, -3.5);
	bia3();
	model_bia = Translate(0, 1.2,-3.5);
	bia3();
	model_bia = Translate(-4, 1.2, -3.5);
	bia3();
	
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{	
	switch (key) {
		//Cua DAOLINH
	case '1':
		buoctruoc();
		break;
	case '!':
		buocsau();
		break;
	case '2':
		buoctrai();
		break;
	case '@':
		buocphai();
		break;
	case '3':
		dz += 10;
		if (dz > 30) {
			dz = 0;
		}
		glutPostRedisplay();
		break;
	case '#':
		dz -= 10;
		if (dz < -30) {
			dz = 0;
		}
		glutPostRedisplay();
		break;
	case '4':
		beta[2] += 5;
		glutPostRedisplay();
		break;
	case '$':
		beta[2] -= 5;
		glutPostRedisplay();
		break;
	case '5':
		beta[1] += 5;
		glutPostRedisplay();
		break;
	case '%':
		beta[1] -= 5;
		glutPostRedisplay();
		break;
	case '6':
		beta[6] += 5;
		if (beta[6] > 180) beta[6] = 180;
		glutPostRedisplay();
		break;
	case '^':
		beta[6] -= 5;
		if (beta[6] < 0) beta[6] = 0;
		glutPostRedisplay();
		break;
	case '7':
		beta[3] += 5;
		glutPostRedisplay();
		break;
	case '&':
		beta[3] -= 5;
		glutPostRedisplay();
		break;
	case '8':
		if (phai[0] < 40)
		{
			phai[0] += 40;
			phai[1] += 20;
			phai[2] += 15;
			phai[4] += 100;
			phai[6] -= 30;
			phai[3] += 20;
			sung[0] -= 90;
			sung[1] += 60;
			sung[2] += 10;

			beta[1] += 30;
			beta[2] += 30;
			beta[5] -= 100;
			beta[6] += 130;
			beta[7] += 50;
			s2 += 0.4;
			s3 += 0.2;
		}
		glutPostRedisplay();
		break;
	case '*':
		if (phai[0] > 10)
		{
			s3 -= 0.2;
			s2 -= 0.4;
			beta[7] -= 50;
			beta[6] -= 130;
			beta[5] += 100;
			beta[2] -= 30;
			beta[1] -= 30;
			sung[2] -= 10;
			sung[1] -= 60;
			sung[0] += 90;
			phai[3] -= 20;
			phai[6] += 30;
			phai[4] -= 100;
			phai[2] -= 15;
			phai[1] -= 20;
			phai[0] -= 40;
		}
		glutPostRedisplay();
		break;
	case '9':			// 033 is Escape key octal value
		bb -= 35;
		if (bb < -20) bb = 15;
		glutIdleFunc(banphao_linh);
		glutPostRedisplay();
		break;
	case '(':			// 033 is Escape key octal value
		bb -= 35;
		if (bb < -20) bb = 15;
		cd = 0;
		glutIdleFunc(banphao_linh);
		break;
	case '0':
		nguoi[0] -= 5;
		glutPostRedisplay();
		break;
	case ')':
		nguoi[0] += 5;
		glutPostRedisplay();
		break;



		//Cua MANH

	case 'q':
		thetamanh[9] += 5;
		if (thetamanh[9] > 30) thetamanh[9] = 0;
		glutPostRedisplay();
		break;
		//hạ pháo 
	case 'Q':
		thetamanh[9] -= 5;
		if (thetamanh[9] <= -30) thetamanh[9] = 0;
		glutPostRedisplay();
		break;
		// đổi góc camera
	case 'w':
		thetamanh[1] += 5;
		if (thetamanh[1] > 360) thetamanh[1] = 0;
		glutPostRedisplay();
		break;
	case 'W':
		thetamanh[1] -= 5;
		if (thetamanh[1] < -360) thetamanh[1] = 0;
		glutPostRedisplay();
		break;
		//xe tiến 
	case 'r':
		thetamanh[3] -= 15;
		Sleep(50);
		pull -= 0.05;
		glutPostRedisplay();
		break;
		// xe lùi 
	case 'R':
		thetamanh[3] += 15;
		Sleep(50);
		pull += 0.05;
		glutPostRedisplay();
		break;
		// mở cửa trái 
	case 't':
		thetamanh[5] -= 5;
		if (thetamanh[5] <= -80)
		{
			thetamanh[5] = -80;
		}
		glutPostRedisplay();
		break;
		// đóng cửa trái 
	case 'T':
		thetamanh[5] += 5;
		if (thetamanh[5] >= 0)
		{
			thetamanh[5] = 0;
		}

		glutPostRedisplay();
		break;
		// mở cửa phải
	case 'e':
		thetamanh[6] += 5;
		if (thetamanh[6] >= 80)
		{
			thetamanh[6] = 80;
		}
		glutPostRedisplay();
		break;
		// đóng cửa phải
	case 'E':
		thetamanh[6] -= 5;
		if (thetamanh[6] <= 0)
		{
			thetamanh[6] = 0;
		}
		glutPostRedisplay();
		break;
		// quay pháo 
	case 'y':
		thetamanh[7] -= 5;
		if (thetamanh[7] <= -360)
		{
			thetamanh[7] = 0;
		}
		glutPostRedisplay();
		break;
	case 'Y':
		thetamanh[7] += 5;
		if (thetamanh[7] >= 360)
		{
			thetamanh[7] = 0;
		}
		glutPostRedisplay();
		break;
		// bắn pháo đây nè 
	case 'u':			// 033 is Escape key octal value
		/*bb -= 35;
		if (bb < -20) bb = 15;*/
		glutIdleFunc(banphao);
		glutPostRedisplay();
		break;
	case 'U':			// 033 is Escape key octal value
		/*bb -= 35;
		if (bb < -20) bb = 15;*/
		cdmanh = 0;
		glutPostRedisplay();
		break;
	// Của tâm 
	

	case 's':
		tamta[1] += 5;
		if (tamta[1] > 360) tamta[1] = 0;
		glutPostRedisplay();
		break;
	case 'S':
		tamta[1] -= 5;
		if (tamta[1] < -360) tamta[1] = 0;
		glutPostRedisplay();
		break;
	case 'j':
		tamta[3] -= 15;
		Sleep(50);
		pull_tam -= 0.05;
		glutPostRedisplay();
		break;
	case 'J':
		tamta[3] += 15;
		Sleep(50);
		pull_tam += 0.05;
		glutPostRedisplay();
		break;
	case 'k':
		tamta[5] -= 5;
		if (tamta[5] <= -80)
		{
			tamta[5] = -80;
		}
		glutPostRedisplay();
		break;
	case 'K':
		tamta[5] += 5;
		if (tamta[5] >= 0)
		{
			tamta[5] = 0;
		}

		glutPostRedisplay();
		break;
	case 'd':
		tamta[6] += 5;
		if (tamta[6] >= 80)
		{
			tamta[6] = 80;
		}
		glutPostRedisplay();
		break;
	case 'D':
		tamta[6] -= 5;
		if (tamta[6] <= 0)
		{
			tamta[6] = 0;
		}
		glutPostRedisplay();
		break;
	case 'f':
		tamta[7] -= 5;
		if (tamta[7] <= -180)
		{
			tamta[7] = -180;
		}
		glutPostRedisplay();
		break;
	case 'F':
		tamta[7] += 5;
		if (tamta[7] >= 180)
		{
			tamta[7] = 180;
		}
		glutPostRedisplay();
		break;
	case 'h':
		v_t = 0.5;
		glutPostRedisplay();
		break;
	case 'l':
		tamta[8] -= 5;
		if (tamta[8] <= -50)
		{
			tamta[8] = -50;
		}
		glutPostRedisplay();
		break;
	case 'L':
		tamta[8] += 5;
		if (tamta[8] >= 0)
		{
			tamta[8] = 0;
		}
		glutPostRedisplay();
		break;

// CỦA DUYÊN 


	case 'z':
		dr_D += 5;
		glutPostRedisplay();
		break;

		//phải
	case 'Z':
		dr_D -= 5;
		glutPostRedisplay();
		break;

		// bắn đạn
	case 'x':
		glutIdleFunc(bandan_D);
		glutPostRedisplay();
		break;

	case 'X':
		s_D = 0;
		glutPostRedisplay();
		break;

		// đi lên
	case 'c':
		xx_D += 0.05;
		theta_D[3] += 1.5;
		glutPostRedisplay();
		break;

		// đi lùi
	case 'C':
		xx_D -= 0.05;
		theta_D[3] -= 1.5;
		glutPostRedisplay();
		break;

		// mở nắp
	case 'v':
		theta_D[1] -= 5;
		if (theta_D[1] < -90) theta_D[1] = -90;
		glutPostRedisplay();
		break;
		// đóng
	case 'V':
		theta_D[1] += 5;
		if (theta_D[1] > 0) theta_D[1] = 0;
		glutPostRedisplay();
		break;

		// nâng pháo
	case 'b':
		theta_D[2] += 5;
		if (theta_D[2] > 35) theta_D[2] = 35;
		glutPostRedisplay();
		break;

		// hạ
	case 'B':
		theta_D[2] -= 5;
		if (theta_D[2] < 0) theta_D[2] = 0;
		glutPostRedisplay();
		break;

	case 'n':
		theta_D[3] += 5;
		glutPostRedisplay();
		break;


		// CỦA HOÀI 

	case ',':
		theta_h[0] += 5;
		break;
	case '.':
		theta_h[0] -= 5;
		break;
	case '{':	// mo cua
		theta_h[1] -= 5;
		if (theta_h[1] < -180) theta_h[1] = -180;
		break;
	case '}':	// dong cua
		theta_h[1] += 5;
		if (theta_h[1] > 0) theta_h[1] = 0;
		break;
	case '[':	// ha cua kinh
		yy_h -= 0.01;
		if (yy_h < -0.2) yy_h = -0.2;
		break;
	case ']':	// ha cua kinh
		yy_h += 0.01;
		if (yy_h > 0) yy_h = 0;
		break;
	case '+':	// tien
		theta_h[2] += 5;
		xx_H -= 0.1;

		break;
	case '-':	// lui
		theta_h[2] -= 5;
		xx_H += 0.1;

		break;
	

	case '/':
		c+= 0.2; glutPostRedisplay();
		break;
	case '?':
		c -= 0.2; glutPostRedisplay();
		break;
	case 'i':
		a += 0.2; glutPostRedisplay();
		break;
	case 'I':
		a -= 0.2; glutPostRedisplay();
		break;
	case 'o':
		zNear *= 0.9; zFar *= 0.9; glutPostRedisplay();  break;
	case 'O':
		zNear *= 1.1; zFar *= 1.1; glutPostRedisplay();  break;
	case 'p':
		b += 0.2; glutPostRedisplay();
		break;
	case 'P':
		b -= 0.2; glutPostRedisplay();
		break;
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	}	

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(50, 0);
	glutCreateWindow("Mo Hinh Tap Tran Xe Bo Binh");
	cout << "Cac Phim Dieu Khien Xe BM_21" << endl;
	//..
	cout << "Cac Phim Dieu Khien Xe ZTL-11" << endl;

	cout << "Cac Phim Dieu Khien Xe Humvee" << endl;

	cout << "Cac Phim Dieu Khien Xe Tang" << endl;

	cout << "Cac Phim Dieu Khien Xe BF-11A" << endl;

	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutIdleFunc(ban_T);
	//glutIdleFunc(ban_Truong);
	//glutIdleFunc(fire_Quan);
	//glutIdleFunc(spin_Khanh);
	//glutIdleFunc(spin_Khanh);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
