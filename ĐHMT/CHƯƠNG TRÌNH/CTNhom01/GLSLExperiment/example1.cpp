//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

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

point4 points[NumPoints];              /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints];              /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints];               /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/

point4 vertices[8];                    /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8];               /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;
GLuint model_loc, view_loc, projection_loc;
mat4 cmt;                              //la model_phong

GLfloat anphal;                        // goc xoay cua chinh
GLfloat  beta;                         // goc quay phong
GLfloat xx, yy, zz;                    // di chuyen cam
	// thong so view
GLfloat xeye = 0, yeye = 1, zeye = 5;
GLfloat xup = 0, zup = 0;
GLfloat yat = 1;
GLfloat thetal = 179;                  // goc quay camera theo truc Y
GLfloat radius;                        // goc quay camera theo truc Z

	// thong so projection
GLfloat l = -1.0, r = 1.0, t = 1.0, b = -1.0, zNear = 0.5, zFar = 6;

GLfloat z1, z2;                       // di chuyen cua so
GLfloat x1, x2;                       // dong mo ngan keo tu quan ao
GLboolean check = false;              // kiem tra hoat dong  quat
GLfloat xoayquat;                     // quay canh quat
GLfloat xGiuong;                      // di chuyen ngan keo giuong
GLfloat x_ghe, y_ghe, z_ghe;          // di chuyen ghe
GLfloat xoaycuahoctu;                 // mo cua canh hoc tu
GLboolean check_den = false;          // kiem tra trang thai bong den
GLfloat delta = 0, phi1 = 0, phi2 = 0;


mat4 Model_cua_chinh;
mat4 canh_cua;
mat4 Model_cua_so;
mat4 Model_canhcuahoctu;
mat4 Model_ghe;
mat4 Model_quat;
mat4 Model_giuong;
mat4 Model_tu;
mat4 canh_tu1, canh_tu2;


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
	vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0); // white
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

	/* Khởi tạo các tham số chiếu sáng - tô bóng*/

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}


//bảng phối màu
//màu tường nhà
void powderblue()
{
	point4 light_position(0.7, 0.88, 0.9, 1.0);
	color4 light_ambient(0.7, 0.88, 0.9, 1.0);
	color4 light_diffuse(0.7, 0.88, 0.9, 1.0);
	color4 light_specular(0.7, 0.88, 0.9, 1.0);

	color4 material_ambient(0.7, 0.88, 0.9, 1.0);
	color4 material_diffuse(0.1, 0.2, 0.2, 0.5);
	color4 material_specular(0.7, 0.88, 0.9, 1.0);
	float material_shininess = 100000.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

void mau(double x, double y, double z) {
	point4 light_position(0, 0, 1, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1, 1, 1, 1.0);
	color4 light_specular(1, 1, 1, 1.0);

	color4 material_ambient(1.0, 1, 1.0, 1.0);
	color4 material_diffuse(x, y, z, 1.0);
	color4 material_specular(1.0, 1, 1, 1.0);
	float material_shininess = 100000.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

void milk()
{
	point4 light_position(0.96, 0.96, 0.96, 0.0);
	color4 light_ambient(0.96, 0.96, 0.96, 1.0);
	color4 light_diffuse(0.96, 0.96, 0.96, 1.0);
	color4 light_specular(0.96, 0.96, 0.96, 1.0);

	color4 material_ambient(0.96, 0.96, 0.96, 1.0);
	color4 material_diffuse(0.96, 0.96, 0.96, 1.0);
	color4 material_specular(0.96, 0.96, 0.96, 1.0);
	float material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

void xanhla() {
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.329412, 0.223529, 0.027451, 1.0);
	color4 material_diffuse(0.5, 1, 0.213, 1.0);
	color4 material_specular(0.213, 0.189, 0.159, 1.0);
	float material_shininess = 25;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

//màu trần nhà
void trang() {
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.2, 0.2, 0.2, 1.0);
	color4 material_diffuse(1, 1, 1, 1.0);
	color4 material_specular(1, 1, 1, 1.0);
	float material_shininess = 100;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

void trang_bong_den() {
	point4 light_position(0, 1, 0, 1);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.2, 0.2, 0.2, 1.0);
	color4 material_diffuse(1, 1, 1, 1.0);
	color4 material_specular(1, 1, 1, 1.0);
	float material_shininess = 0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

//màu cửa sổ
void nau() {
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.2, 0.2, 0.2, 1.0);
	color4 material_diffuse(0.262, 0.219, 0.268, 1.0);
	color4 material_specular(0.4, 0.4, 0.4, 1.0);
	float material_shininess = 70;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

void xam() {
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 0.0);

	color4 material_ambient(0.2, 0.2, 0.2, 1.0);
	color4 material_diffuse(0.5, 0.5, 0.5, 0.5);
	color4 material_specular(0.4, 0.4, 0.4, 0.0);
	float material_shininess = 10000;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

void xanhngoc() {
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.2, 0.2, 0.2, 1.0);
	color4 material_diffuse(0.54, 0.89, 0.63, 1.0);
	color4 material_specular(0.316, 0.316, 0.316, 1.0);
	float material_shininess = 16;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

void naunhe() {
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.2, 0.2, 0.2, 1.0);
	color4 material_diffuse(0.45, 0.27, 0, 1.0);
	color4 material_specular(0.257, 0.138, 0.086, 1.0);
	float material_shininess = 25;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

}

void naugo() {
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.2, 0.2, 0.2, 1.0);
	color4 material_diffuse(1, 0.695, 0.213, 1.0);
	color4 material_specular(0.213, 0.189, 0.159, 1.0);
	float material_shininess = 25;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

}


//-----------------------------Phạm Minh Hòa---------------------------------
//----------------- Nhà
void sannha(GLfloat x, GLfloat y, GLfloat z) {
	mat4 sannha = Translate(x, y, z) * Scale(3, 0.015, 7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * sannha);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void tuong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 tuong = Translate(x, y, z) * Scale(0.02, 4, 7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * tuong);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongsau(GLfloat x, GLfloat y, GLfloat z) {
	mat4 tuong = Translate(x, y, z) * Scale(3, 4, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * tuong);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

// canh cua chinh
void thanhngang(GLfloat x, GLfloat y, GLfloat z) {
	mat4 cua = Translate(x, y, z) * Scale(1.02, 0.06, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_chinh * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanhdung1(GLfloat x, GLfloat y, GLfloat z) {
	mat4 cua = Translate(x, y, z) * Scale(0.06, 1.8, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_chinh * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanhdung2(GLfloat x, GLfloat y, GLfloat z) {
	mat4 cua = Translate(x, y, z) * Scale(0.05, 1.7, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_chinh * canh_cua * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void kinhcua() {
	mat4 cua = Translate(0, 0.91, 0) * Scale(0.9, 1.7, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_chinh * canh_cua * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void truckhoa() {
	mat4 cua = Translate(-0.3, 0.8, 0) * Scale(0.04, 0.04, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_chinh * canh_cua * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void taycam(GLfloat x, GLfloat y, GLfloat z) {
	mat4 cua = Translate(x, y, z) * Scale(0.175, 0.03, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_chinh * canh_cua * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void cuaravaotrai() {
	mau(0, 0, 0);
	thanhngang(0, 1.8, 0); //thanh ngang trên
	thanhngang(0.02, 0.03, 0); //thanh ngang dưới
	thanhdung1(0.5, 0.9, 0); //thanh dọc phải

	canh_cua = Translate(0.45, 0, 0) * RotateY(anphal) * Translate(-0.45, 0, 0);
	mau(0, 0, 0);
	thanhdung2(-0.45, 0.91, -0.02); //thanh dọc trái //---->
	mau(0.7, 0.7, 0.7);
	kinhcua();
	mau(0, 0, 0);
	truckhoa();
	taycam(-0.25, 0.8, -0.1);
	taycam(-0.25, 0.8, 0.1);
}

void cuaravaophai() {
	mau(0, 0, 0);
	thanhngang(-0.01, 1.8, 0); //thanh ngang trên
	thanhngang(-0.01, 0.03, 0); //thanh ngang dưới
	thanhdung1(-0.48, 0.9, 0); //thanh dọc trái
	canh_cua = Translate(-0.45, 0, 0) * RotateY(-anphal) * Translate(0.45, 0, 0);
	mau(0.7, 0.7, 0.7);

	kinhcua();
}

void tuongdoc1(GLfloat x, GLfloat y, GLfloat z) {
	mat4 tuong = Translate(x, y, z) * Scale(3, 2.2, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * tuong);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongdoc2(GLfloat x, GLfloat y, GLfloat z) {
	mat4 tuong = Translate(x, y, z) * Scale(0.52, 1.8, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * tuong);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

// Khung cua so

void thanhngangcuaso(GLfloat x, GLfloat y, GLfloat z) {
	mat4 cua = Translate(x, y, z) * Scale(0.06, 0.06, 0.75);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanhdoccuaso(GLfloat x, GLfloat y, GLfloat z) {
	mat4 cua = Translate(x, y, z) * Scale(0.06, 1, 0.06);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void kinhcuaso() {
	mat4 cuaso = Scale(0.04, 0.94, 0.63);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_so * cuaso);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void taycamcuaso(GLfloat x, GLfloat y, GLfloat z) {
	mat4 cuaso = Translate(x, y, z) * Scale(0.15, 0.03, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_so * cuaso);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tayquaycuaso(GLfloat x, GLfloat y, GLfloat z) {
	mat4 cuaso = Translate(x, y, z) * Scale(0.03, 0.03, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_cua_so * cuaso);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void cuaso() {

	mau(0, 0, 0);
	thanhngangcuaso(-1.5, 2.5, 1.65);
	thanhdoccuaso(-1.5, 2, 2);
	thanhdoccuaso(-1.5, 2, 1.3);
	thanhngangcuaso(-1.5, 1.5, 1.65);



	mau(0.7, 0.7, 0.7);
	Model_cua_so = Translate(-1.5, 2, 1.65) * Translate(0, 0.47, 0) * RotateZ(-delta) * Translate(0, -0.47, 0);
	kinhcuaso();


	mau(0, 0, 0);
	taycamcuaso(0.07, -0.3, 0);
	tayquaycuaso(0.1, -0.3, 0.05);
}
void tuongbao1(GLfloat x, GLfloat y, GLfloat z) {
	mat4 tuongbao = Translate(x, y, z) * Scale(0.02, 4, 4.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * tuongbao);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongbao2(GLfloat x, GLfloat y, GLfloat z) {
	mat4 tuongbao = Translate(x, y, z) * Scale(0.02, 4, 1.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * tuongbao);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongbao3(GLfloat x, GLfloat y, GLfloat z) {
	mat4 tuongbao = Translate(x, y, z) * Scale(0.02, 1.5, 0.75);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * tuongbao);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void phong() {
	mau(0.222 * 7, 0.184 * 7, 0.135 * 7);
	sannha(0, 0, 0);

	powderblue();
	tuong(1.5, 2, 0);

	trang();
	sannha(0, 4, 0); //trần nhà

	powderblue();
	tuongsau(0, 2, -3.5);

	Model_cua_chinh = Translate(0.47, 0, 3.52);
	cuaravaotrai();

	Model_cua_chinh = Translate(-0.46, 0, 3.52);
	cuaravaophai();

	powderblue();
	tuongdoc1(0, 2.9, 3.5);
	tuongdoc2(1.25, 0.9, 3.5);
	tuongdoc2(-1.25, 0.9, 3.5);


	cuaso();

	powderblue();
	tuongbao1(-1.5, 2, -1.1);
	tuongbao2(-1.5, 2, 2.75);
	tuongbao3(-1.5, 0.75, 1.65);
	tuongbao3(-1.5, 3.25, 1.65);
}
//----------------------------------Lê Thành An-------------------------------------
//---------------- Bàn 
void chanban(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.04, 0.8, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void matban(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.7, 0.03, 1.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void dehoctuban(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.6, 0.04, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lunghoctuban(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.6, 0.8, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void ophoctuban(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.04, 0.8, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canhhoctuban() {
	mat4 chan = Scale(0.04, 0.8, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_canhcuahoctu * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void hoctuban() {
	dehoctuban(-1.1, 0.05, 1.34);
	lunghoctuban(-1.1, 0.46, 1);
	lunghoctuban(-1.1, 0.46, 1.75);
	ophoctuban(-1.4, 0.46, 1.34);
	Model_canhcuahoctu = Translate(-0.8, 0.46, 1.34) * Translate(0, 0, -0.4) * RotateY(xoaycuahoctu) * Translate(0, 0, 0.4);
	canhhoctuban();
}

void ban() {
	chanban(-1.4, 0.46, 1);
	chanban(-0.8, 0.46, 2.5);
	chanban(-1.4, 0.46, 2.5);
	chanban(-0.8, 0.46, 1);
	matban(-1.1, 0.87, 1.75);
	hoctuban();
}

// Ghế
void changhe(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.04, 0.4, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_ghe * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void matghe(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.6, 0.03, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_ghe * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ghetua(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.03, 0.7, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_ghe * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ghe() {
	Model_ghe = Translate(x_ghe, y_ghe, z_ghe);
	naugo();
	changhe(-0.6, 0.23, 2);
	changhe(-0.6, 0.23, 1.6);
	changhe(-0.1, 0.23, 2);
	changhe(-0.1, 0.23, 1.6);
	matghe(-0.35, 0.43, 1.8);
	ghetua(-0.1, 0.75, 1.8);
}


// Tủ quần áo
void chantu(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.03, 0.3, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void detu(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.8, 0.04, 1.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lungtucao(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.02, 1.75, 1.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hongtucao(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.8, 1.8, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void noctucao(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.8, 0.04, 1.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanhtreo(GLfloat x, GLfloat y, GLfloat z) {
	mat4 den = Translate(x, y, z) * Scale(0.04, 0.04, 1.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * den);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanhngan(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.8, 0.04, 1.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void canhtucao1() {
	mat4 chan = Scale(0.02, 1.4, 0.9);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * canh_tu1 * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canhtucao2() {
	mat4 chan =  Scale(0.02, 1.4, 0.9);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * canh_tu2 * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void opngankeo(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.02, 0.3, 1.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_tu * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhngankeo(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.6, 0.3, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_tu * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void dengankeo(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.8, 0.04, 1.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_tu * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ngankeo01() {
	Model_tu = Translate(x1, 0, 0);
	canhngankeo(-0.9, 0.4, -3.2);
	canhngankeo(-0.9, 0.4, -1.45);
	dengankeo(-1, 0.22, -2.35);
	opngankeo(-0.6, 0.4, -2.3);

}

void tuquanao() {
	chantu(-1.3, 0.05, -3);
	chantu(-1.3, 0.05, -1.6);
	chantu(-0.75, 0.05, -3);
	chantu(-0.75, 0.05, -1.6);
	detu(-1, 0.2, -2.35);
	lungtucao(-1.4, 1.1, -2.3);
	noctucao(-1, 2, -2.3);
	hongtucao(-1, 1.13, -3.2);
	hongtucao(-1, 1.13, -1.4);
	thanhngan(-1, 0.55, -2.3);
	
	
	canhtucao1();
	canh_tu1 = Translate(-0.58, 1.3, -2.7) * Translate(0, 0, -0.43) * RotateY(phi1) * Translate(0, 0, 0.43);
	
	canhtucao2();
	canh_tu2 = Translate(-0.58, 1.3, -1.8) * Translate(0, 0, 0.42) * RotateY(-phi2) * Translate(0, 0, -0.42);

	ngankeo01();


	trang();
	thanhtreo(-1, 1.54, -2.3);


}


//-------------------------------Trần Quang Dũng----------------------------------
//--------------  Giường
void caigiuong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(1, 0.1, 2.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void daugiuong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(1, 0.5, 0.05);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void goi(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.5, 0.04, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void duoigiuong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(1, 0.32, 0.05);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void dengankkeogiuong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.8, 0.03, 2.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_giuong * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhngankkeogiuong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.03, 0.22, 2.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_giuong * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void opngankkeogiuong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(0.8, 0.22, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_giuong * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ngankeogiuong() {
	dengankkeogiuong(0.9, 0.12, -2.2);
	canhngankkeogiuong(0.48, 0.2, -2.2);
	canhngankkeogiuong(1.4, 0.2, -2.2);
	opngankkeogiuong(0.9, 0.2, -3.3);
	opngankkeogiuong(0.9, 0.2, -1.05);
}

void dem(GLfloat x, GLfloat y, GLfloat z) {
	mat4 chan = Translate(x, y, z) * Scale(1, 0.1, 2.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * chan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void giuong() {
	naugo();
	caigiuong(0.95, 0.325, -2.2);
	caigiuong(0.95, 0.05, -2.2);
	daugiuong(0.95, 0.35, -3.4);
	duoigiuong(0.95, 0.2, -0.95);

	trang();
	dem(0.95,0.35,-2.2);

	xam();
	goi(0.95, 0.4, -3.2);

	naugo();
	Model_giuong = Translate(-xGiuong, 0, 0);
	ngankeogiuong();
}


// Đèn phòng 
void deden(GLfloat x, GLfloat y, GLfloat z) {
	mat4 den = Translate(x, y, z) * Scale(0.03, 0.1, 2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * den);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tai(GLfloat x, GLfloat y, GLfloat z) {
	mat4 den = Translate(x, y, z) * Scale(0.1, 0.1, 0.07);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * den);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void bong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 den = Translate(x, y, z) * Scale(0.05, 0.05, 1.86);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * den);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void batden() {
	if (check_den) {
		trang_bong_den();
		bong(1.4, 3, 0);
		glutPostRedisplay();
	}
	else {
		trang();
		bong(1.4, 3, 0);
		glutPostRedisplay();
	}
}

void den() {
	deden(1.47, 3, 0);
	tai(1.4, 3, 0.95);
	tai(1.4, 3, -0.95);
	batden();
}


// Quạt Trần
void trucquat(GLfloat x, GLfloat y, GLfloat z) {
	mat4 quat = Translate(x, y, z) * Scale(0.03, 0.5, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * quat);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhquat(GLfloat x, GLfloat y, GLfloat z, GLfloat t) {
	mat4 quat = Translate(x, y, z) * RotateY(t) * Translate(0.25, 0, 0) * Scale(0.5, 0.01, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, cmt * Model_quat * quat);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void quay() {
	if (check) {
		xoayquat += 5;
		glutPostRedisplay();
	}
	else {
		xoayquat = 0;
		glutPostRedisplay();
	}
}

void quattran() {
	trucquat(0, 3.2, 0);
	Model_quat *= RotateY(xoayquat);
	canhquat(0, 2.95, 0, 0);
	canhquat(0, 2.95, 0, 120);
	canhquat(0, 2.95, 0, -120);
	quay();
}


void camera()
{
	point4 eye(xeye, yeye, zeye, 1.0);
	point4 at(xeye + sin(thetal), yat, cos(thetal) + zeye, 1.0);
	vec4 up(sin(radius), cos(radius), zup, 1.0);

	mat4 view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	mat4 p = Frustum(l, r, b, t, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);

}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera();

	phong();

	giuong();

	ban();

	den();

	ghe();

	naunhe();
	tuquanao();

	xanhngoc();
	quattran();

	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
		//quay cửa ra vào
	case 'q':
		anphal -= 5;
		if (anphal < -90) {
			anphal = -90;
		}
		break;
	case 'Q':
		anphal += 5;
		if (anphal > 0) {
			anphal = 0;
		}
		break;


	case 'x':
		beta += 5;
		break;

	case 'X':
		beta -= 5;
		break;

		//cửa sổ
	case't':
		delta += 5;
		if (delta > 50) {
			delta = 50;
		}
		break;
	case'T':
		delta -= 5;
		if (delta < 0) {
			delta = 0;
		}
		break;
		//di chuyển cam
	case 'u':
		yat += 0.1;
		break;
	case 'U':
		yat -= 0.1;
		break;
	case 'd': thetal -= 0.1; break;
	case 'a': thetal += 0.1; break;
	case 'f': radius += 0.1; break;
	case 'F': radius -= 0.1; break;
	case 's': zeye += 0.1; break;
	case 'w': zeye -= 0.1; break;
	case 'z': zNear *= 1.1; zFar *= 1.1; break;
	case 'Z': zNear *= 0.9; zFar *= 0.9; break;
		//cánh tủ cao
	case 'm':
		phi1 += 10;
		if (phi1 > 90) {
			phi1 = 90;
		}
		break;
	case 'M':
		phi1 -= 10;
		if (phi1 < 0) {
			phi1 = 0;
		}
		break;
	case 'n':
		phi2 += 10;
		if (phi2 > 90) {
			phi2 = 90;
		}
		break;
	case 'N':
		phi2 -= 10;
		if (phi2 < 0) {
			phi2 = 0;
		}
		break;
		//ngăn tủ dưới của tủ quan ao
	case '1':
		x1 += 0.06;
		if (x1 > 0.5) {
			x1 = 0.5;
		}
		break;
	case '!':
		x1 -= 0.06;
		if (x1 < 0) {
			x1 = 0;
		}
		break;

		//quay quạt trần
	case 'e':
		check = !check;
		break;
		//ngăn kéo của giường
	case 'k':
		xGiuong += 0.05;
		if (xGiuong > 0.7) {
			xGiuong = 0.7;
		}
		break;
	case 'K':
		xGiuong -= 0.05;
		if (xGiuong < 0) {
			xGiuong = 0;
		}
		break;
		// mở ngăn tủ của hộc bàn
	case 'r':
		xoaycuahoctu += 5;
		if (xoaycuahoctu > 90) {
			xoaycuahoctu = 90;
		}
		break;
	case 'R':
		xoaycuahoctu -= 5;
		if (xoaycuahoctu < 0) {
			xoaycuahoctu = 0;
		}
		break;
		// di chuyen ghe
	case 'y':
		z_ghe += 0.05;
		if (z_ghe > 1.2) {
			z_ghe = 1.2;
		}
		break;
	case 'Y':
		z_ghe -= 0.05;
		if (z_ghe < -5) {
			z_ghe = -5;
		}
		break;
	case 'l':    // bat den
		check_den = !check_den;
		break;

	}
	glutPostRedisplay();
}

void danhsachlenh() {
	cout << "q : mo cua chinh " << endl;
	cout << "Q : dong cua chinh " << endl;
	cout << "t : mo cua so " << endl;
	cout << "T : dong cua so" << endl;
	cout << "x, X : xoay ca phong " << endl;
	cout << "u : nhin len  " << endl;
	cout << "U : nhin xuong  " << endl;
	cout << "w : tien camera len  " << endl;
	cout << "s : lui camera xuong  " << endl;
	cout << "a : xoay camera sang trai  " << endl;
	cout << "s : xoay camera sang phai  " << endl;
	cout << "z : phong to " << endl;
	cout << "Z : thu nho " << endl;
	cout << "m , M; n , N : mo, dong canh tu quan ao " << endl;
	cout << "1 , ! : mo, dong ngan keo tu quan ao " << endl;
	cout << "e : bat, tat quat tran " << endl;
	cout << "k, K : mo, dong ngan keo giuong " << endl;
	cout << "r, R : mo dong canh tu ban " << endl;
	cout << "y, Y : di chuyen ghe " << endl;
	cout << "l : bat, tat den dien " << endl;
	cout << "f, F : xoay camera theo ,nguoc chieu kim dong ho " << endl;
	cout << endl;
}

int main(int argc, char** argv)
{
	// main function: program starts here
	danhsachlenh();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Mô hình phòng ngủ");

	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
