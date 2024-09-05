/*Chương trình chiếu sáng Blinn-Phong (Phong sua doi) cho hình lập phương đơn vị, điều khiển quay bằng phím x, y, z, X, Y, Z.*/

#include "Angel.h" /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/

// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

typedef vec4 point4;
typedef vec4 color4;
using namespace std;

const int NumPoints = 36;
const int ii = 30, jj = 20;
const int diemHinhLapPhuong = 36;
const int diemHinhTru = 720;
const int diemHinhTron = 4462;
const int n = 30;
double PI = 3.14;
const int sum = diemHinhLapPhuong + diemHinhTru + diemHinhTron;
point4 points[sum]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[sum]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[sum];	/*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/

point4 vertices[8];		 /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;

//GLfloat theta[3] = {0, 0, 0};
GLfloat dr = 5;

mat4 model;
GLuint model_loc;
mat4 projection;
GLuint projection_loc;
mat4 view;
GLuint view_loc;

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
void quad(int a, int b, int c, int d) /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal;
	colors[Index] = vertex_colors[a];
	points[Index] = vertices[a];
	Index++;
	normals[Index] = normal;
	colors[Index] = vertex_colors[a];
	points[Index] = vertices[b];
	Index++;
	normals[Index] = normal;
	colors[Index] = vertex_colors[a];
	points[Index] = vertices[c];
	Index++;
	normals[Index] = normal;
	colors[Index] = vertex_colors[a];
	points[Index] = vertices[a];
	Index++;
	normals[Index] = normal;
	colors[Index] = vertex_colors[a];
	points[Index] = vertices[c];
	Index++;
	normals[Index] = normal;
	colors[Index] = vertex_colors[a];
	points[Index] = vertices[d];
	Index++;
}
void add(point4 A, point4 B, point4 C)
{
	vec4 u = B - A;
	vec4 v = C - B;
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal;
	points[Index] = A;
	colors[Index] = vertex_colors[2];
	Index++;
	normals[Index] = normal;
	points[Index] = B;
	colors[Index] = vertex_colors[7];
	Index++;
	normals[Index] = normal;
	points[Index] = C;
	colors[Index] = vertex_colors[2];
	Index++;
}
void quadHinhTru(int n, float r1, float r2, float h, float h2, int color)
{
	const float apha = (float)(2 * PI) / n;
	for (int i = 0; i < n; i++)
	{
		vec4 A = point4(r1 * cos((i - 2) * apha), h, r1 * sin((i - 2) * apha), 1.0);
		vec4 B = point4(r1 * cos((i - 1) * apha), h, r1 * sin((i - 1) * apha), 1.0);
		vec4 C = point4(r2 * cos((i - 2) * apha), h2, r2 * sin((i - 2) * apha), 1.0);
		vec4 D = point4(r2 * cos((i - 1) * apha), h2, r2 * sin((i - 1) * apha), 1.0);
		add(A, C, D);
		add(A, D, B);
	}
};

void Sphere()
{
	// double x, y, z;
	float r = (float)0.5;
	const float apha = (float)(2 * PI) / n;
	for (int i = 0; i <= ii; i++)
	{
		for (int j = 0; j <= jj; j++)
		{
			vec4 A = point4(r * sin(i * apha) * cos(j * apha), r * sin(i * apha) * sin(j * apha), r * cos(i * apha), 1.0);
			vec4 B = point4(r * sin((i + 1) * apha) * cos(j * apha), r * sin((i + 1) * apha) * sin(j * apha), r * cos((i + 1) * apha), 1.0);
			vec4 C = point4(r * sin(i * apha) * cos((j + 1) * apha), r * sin(i * apha) * sin((j + 1) * apha), r * cos(i * apha), 1.0);
			vec4 D = point4(r * sin((i + 1) * apha) * cos((j + 1) * apha), r * sin((i + 1) * apha) * sin((j + 1) * apha), r * cos((i + 1) * apha), 1.0);

			add(A, B, C);
			add(B, C, D);
		}
	}
}
void makeColorCube(void) /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void initHinhTru(void)
{

	quadHinhTru(n, 1.0f, 1.0f, -0.5f, 0.5f, 5);
	quadHinhTru(n, 0.5f, 0.5f, -0.5f, 0.5f, 4);
	quadHinhTru(n, 0.5f, 1.0f, 0.5f, 0.5f, 3);
	quadHinhTru(n, 0.5f, 1.0f, -0.5f, -0.5f, 3);
}
void generateGeometry(void)
{
	initCube();
	makeColorCube();
	initHinhTru();
	Sphere();
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
	program = InitShader("vshader1.glsl", "fshader1.glsl"); // hàm InitShader khai báo trong Angel.h
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
	glClearColor(1.0, 1.0, 1.0, 1.0); /* Thiết lập màu trắng là màu xóa màn hình*/
}
vec4 setLight(float a, float b, float c) {
	return vec4(a / 255, b / 255, c / 255, 1.0);
}
point4 light_position(20.0, 50.0f, 1.0, 0.0);
color4 light_ambient = vec4((float)0.2, (float)0.2, (float)0.2, (float)1.0);
color4 light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
color4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);

float material_shininess = 100.0;
void Color(color4 material_ambient, color4 material_diffuse, color4 material_specular, float material)
{
	material_shininess = material;
	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}

/// DAOXUANDONG
class bienDoi
{
public:
	GLfloat translateX, translateY, translateZ;
	GLfloat rotateX, rotateY, rotateZ;
	GLfloat scaleX, scaleY, scaleZ;
	bienDoi()
	{
		translateX = translateY = translateZ = (float)0.0f;
		rotateX = rotateY = rotateZ = (float)0.0f;
		scaleX = scaleY = scaleZ = (float)0.0f;
	}
	bienDoi(float translatex, float translatey, float translatez,
			float rotatex, float rotatey, float rotatez,
			float scalex, float scaley, float scalez)
	{
		translateX = translatex;
		translateY = translatey;
		translateZ = translatez;
		rotateX = rotatex;
		rotateY = rotatey;
		rotateZ = rotatez;
		scaleX = scalex;
		scaleY = scaley;
		scaleZ = scalez;
	}
};
class Control
{
public:
	GLfloat tx;
	GLfloat ty;
	GLfloat tz;
	GLfloat rx;
	GLfloat ry;
	GLfloat rz;
	Control()
	{
		tx = 0;
		ty = 0;
		tz = 0;
		rx = 0;
		ry = 0;
		rz = 0;
	}
	Control(GLfloat txx, GLfloat tyy, GLfloat tzz, GLfloat rxx, GLfloat ryy, GLfloat rzz)
	{
		tx = txx;
		ty = tyy;
		tz = tzz;
		rx = rxx;
		ry = ryy;
		rz = rzz;
	}
};
mat4 instance_home, instance_quat, instance_canhquat, instance_tu;

Control homeController(0, 0, 0, 0, 0, 0);
Control quatController(-1, 3, 0, 0, 0, 0);
Control tuController(0, -2, 0, 0, 0, 0);
Control chassisController(0, 0, 0, 0, 0, 0);
Control trailerController(0, 0, 0, 0, 0, 0);
vec4 setColor(float R, float G, float B) {
	return vec4(R / 255, G / 255, B / 255, 1.0);
}	
color4 ambient;
color4 diffuse;
color4 specular;
// QUAT
void choseColor(float a, float b, float c, float d) {
	ambient = setColor(a, b, c);
	diffuse = setColor(a, b, c);
	specular = setColor(a, b, c);
	Color(ambient, diffuse, specular, d);
}
void hinhTru_quat(bienDoi hinhTru)
{

	ambient = vec4(1.0, 0.0, 1.0, 1.0);
	color4 m1_diffuse = vec4((float)1.0, (float)0.8, (float)0.0, (float)1.0);
	color4 m1_specular = vec4((float)1.0, (float)0.8, (float)0.0, (float)1.0);
	Color(ambient, m1_diffuse, m1_specular, 190);
	mat4 instance = Translate(hinhTru.translateX, hinhTru.translateY, hinhTru.translateZ) * RotateX(hinhTru.rotateX) * RotateY(hinhTru.rotateY) * RotateZ(hinhTru.rotateZ) * Scale(hinhTru.scaleX, hinhTru.scaleY, hinhTru.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_quat * instance);
	glDrawArrays(GL_TRIANGLES, diemHinhLapPhuong, diemHinhTru);
}
void hinhVuong_quat(bienDoi hinhVuong)
{
	ambient = vec4(1.0, 0.0, 1.0, 1.0);
	diffuse = vec4((float)0.3, (float)0.8, (float)0.1, (float)1.0);
	specular = vec4((float)1.0, (float)0.8, (float)0.0, (float)1.0);
	Color(ambient, diffuse, specular, 100);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_quat * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhTron_quat(bienDoi hinhTron)
{
	ambient = vec4(1.0, 0.0, 1.0, 1.0);
	diffuse = vec4((float)0.1, (float)0.3, (float)0.3, (float)1.0);
	specular = vec4((float)1.0, (float)0.8, (float)0.0, (float)1.0);
	Color(ambient, diffuse, specular, 100);
	mat4 instance = Translate(hinhTron.translateX, hinhTron.translateY, hinhTron.translateZ) * RotateX(hinhTron.rotateX) * RotateY(hinhTron.rotateY) * RotateZ(hinhTron.rotateZ) * Scale(hinhTron.scaleX, hinhTron.scaleY, hinhTron.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_quat * instance);
	glDrawArrays(GL_TRIANGLES, diemHinhTru + diemHinhLapPhuong, diemHinhTron);
}
void quat(bienDoi hinhQuat)
{
	mat4 instance = RotateX(hinhQuat.rotateX) * RotateY(hinhQuat.rotateY) * RotateZ(hinhQuat.rotateZ) * Translate(hinhQuat.translateX, hinhQuat.translateY, hinhQuat.translateZ) * Scale(hinhQuat.scaleX, hinhQuat.scaleY, hinhQuat.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_quat * instance_canhquat * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void quatControl()
{
	bienDoi Tru((float)0, (float)0.6, (float)0, 0, 0, 0, (float)0.1, (float)1.0, (float)0.1);
	bienDoi Tron(0, 0, 0, 0, 0, 0, (float)1, (float)0.5, (float)1);
	bienDoi canh1((float)0.75, 0, 0, 0, 0, 0, (float)1.0, (float)0.05, (float)0.35);
	bienDoi canh2((float)0.75, 0, 0, 0, 120, 0, (float)1.0, (float)0.05, (float)0.35);
	bienDoi canh3((float)0.75, 0, 0, 0, 240, 0, (float)1.0, (float)0.05, (float)0.35);

	hinhTron_quat(Tron);
	hinhTru_quat(Tru);
	instance_canhquat =RotateY(quatController.ry);
	quat(canh1);
	instance_canhquat = RotateY(quatController.ry);
	quat(canh2);
	instance_canhquat = RotateY(quatController.ry);
	quat(canh3);
}
// TU
void hinhVuong_Tu(bienDoi hinhVuong)
{
	//= setColor(89, 213, 224);rgb(205, 250, 219)rgb(236, 177, 89)rgb(181, 201, 154)
	choseColor(181, 201, 154, 200);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_tu * instance);

	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void tuControl()
{
	bienDoi canhTren(0, 1, 0, 0, 0, 0, 2, (float)0.02, (float)0.5);
	bienDoi canhDuoi(0, -1, 0, 0, 0, 0, 2, (float)0.02, (float)0.5);
	bienDoi ngan1(0, 0, 0, 0, 0, 0, 2, (float)0.02, (float)0.5);
	bienDoi ngan2(0, (float)0.5, 0, 0, 0, 0, 2, (float)0.02, (float)0.5);
	bienDoi ngan3(0, (float)-0.5, 0, 0, 0, 0, 2, (float)0.02, (float)0.5);
	bienDoi matSau(0, 0, (float)-0.25, 0, 0, 0, 2, 2, (float)0.01);
	bienDoi canhTrai(1, 0, 0, 0, 0, 0, (float)0.01, 2, (float)0.5);
	bienDoi canhPhai(-1, 0, 0, 0, 0, 0, (float)0.01, 2, (float)0.5);
	hinhVuong_Tu(canhTren);
	hinhVuong_Tu(canhDuoi);
	hinhVuong_Tu(ngan1);
	hinhVuong_Tu(ngan2);
	hinhVuong_Tu(ngan3);
	hinhVuong_Tu(canhTrai);
	hinhVuong_Tu(canhPhai);
	hinhVuong_Tu(matSau);
};
// Truck
mat4 truck, instance_truck;
mat4 truckChassis;
mat4 truckTrailer, truckTrailer_Left, truckTrailer_Right, wheels;

void hinhVuong_truckChassis(bienDoi hinhVuong)
{	//RGB(164,116,73)
	
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * truck * instance_truck* instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhVuong_truck(bienDoi hinhVuong)
{
	//RGB(150, 111, 51)
	choseColor(150, 111, 51, 190);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * truck * instance_truck * truckTrailer * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhVuong_truckLeft(bienDoi hinhVuong)
{
	// RGB(161, 102, 47)
	choseColor(161, 102, 47, 190);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * truck* instance_truck * truckTrailer * truckTrailer_Left * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhVuong_truckRight(bienDoi hinhVuong)
{
	choseColor(161, 102, 47, 190);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * truck* instance_truck * truckTrailer * truckTrailer_Right * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhTru_truck(bienDoi hinhTru)
{
	ambient = vec4(1.0, (float)0.1, 1.0, 1.0);
	diffuse = vec4(1.0, (float)0.0, 1.0, 1.0);
	specular = vec4(1.0, (float)0.8, 0.0, 1.0);
	Color(ambient, diffuse, specular, 190);
	mat4 instance = Translate(hinhTru.translateX, hinhTru.translateY, hinhTru.translateZ) * Scale(hinhTru.scaleX, hinhTru.scaleY, hinhTru.scaleZ) * RotateX(hinhTru.rotateX) * RotateY(hinhTru.rotateY) * RotateZ(hinhTru.rotateZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * truck  * instance_truck * instance);
	glDrawArrays(GL_TRIANGLES, diemHinhLapPhuong, diemHinhTru);
}
void hinhVuong_wheels(bienDoi hinhVuong)
{
	choseColor(236, 177, 89, 190);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * truck * instance_truck * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhTru_wheelhinge(bienDoi hinhTru)
{
	ambient = vec4((float)(float)(float)0.8, 1.0, 1.0, 1.0);
	diffuse = vec4(1.0, (float)(float)(float)0.8, (float)(float)(float)0.1, 1.0);
	specular = vec4(1.0, (float)(float)(float)0.8, 0.0, 1.0);
	Color(ambient, diffuse, specular, 190);
	mat4 instance = Translate(hinhTru.translateX, hinhTru.translateY, hinhTru.translateZ) * Scale(hinhTru.scaleX, hinhTru.scaleY, hinhTru.scaleZ) * RotateX(hinhTru.rotateX) * RotateY(hinhTru.rotateY) * RotateZ(hinhTru.rotateZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * truck * instance_truck * instance);
	glDrawArrays(GL_TRIANGLES, diemHinhLapPhuong, diemHinhTru);
}

void hinhTru_wheels(bienDoi hinhTru)
{/*
	bienDoi t(0, 0, 0, 0, 0, 0, (float)0.05, (float)0.4, (float)0.01);
	hinhVuong_wheel(t);*/
	choseColor(236, 177, 89, 190);
	mat4 instance = Translate(hinhTru.translateX, hinhTru.translateY, hinhTru.translateZ) * Scale(hinhTru.scaleX, hinhTru.scaleY, hinhTru.scaleZ) * RotateX(hinhTru.rotateX) * RotateY(hinhTru.rotateY) * RotateZ(hinhTru.rotateZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * truck * instance_truck * wheels * instance);
	glDrawArrays(GL_TRIANGLES, diemHinhLapPhuong, diemHinhTru);
}

void Chassis()
{
	bienDoi box((float)0.8, (float)0.4, 0, 0, 0, 0, 1.0f, 1, 1);
	//bienDoi box2((float)1, (float)0.1, 0, 0, 0, 0, 0.5f, 0.5f, 1);
	bienDoi bottom(0, (float)-0.1, 0, 0, 0, 0, (float)2.5, (float)0.1, 1);
	bienDoi glassis(1.3f, 0.6f, 0.0f, 0, 90, 0, (float)1, (float)0.5, (float)0.01);
	choseColor(164, 116, 73,200);
	hinhVuong_truckChassis(bottom);
	choseColor(164, 116, 73,200);
	hinhVuong_truckChassis(box);
	//hinhVuong_truckChassis(box2);
	choseColor(254, 251, 246,200);
	hinhVuong_truckChassis(glassis);
}
float trailerRx = 0;
float trailerControllerLeft = 0;
float trailerControllerRight = 0;
void Trailer()
{
	bienDoi bottom(-0.5f, 0.65f, 0, 0, 0, 0, 1.5, (float)0.05, (float)1.0);
	bienDoi length1(-0.5f, (float).25, 0.5f, 0, 0, 0, 1.5f, (float)0.75f, (float)0.05);
	bienDoi length2(-0.5f, (float).25,-0.5f, 0, 0, 0, 1.5f, (float)0.75f, (float)0.05);
	//bienDoi width((float)0.475, (float).25, 0, 0, 0, 0, (float).05, (float).5, (float).5);
	bienDoi top(0, (float).5, 0, 0, 0, 0, 1, (float).05, (float)1.5);
	//bienDoi width2((float)0.475, (float).25, 0, 0, 0, 0, (float).05, (float).5, (float).5);
	bienDoi DoorLeft(0, 0, 0, 0, 0, 0, (float).05, (float)0.75, (float).5);
	bienDoi DoorRight(0, 0, 0, 0, 0, 0, (float).05, (float).75, (float).5);
	// bienDoi hinge(0.475, 0, 0.26, 90, 0, 0, 0.03, 0.03, 0.05);
	bienDoi hinge1((float).475, 0, 0, 90, 0, 0, (float).03, (float).03, (float).5);
	hinhVuong_truck(bottom);
	//hinhVuong_truck(top);
	hinhVuong_truck(length1);
	hinhVuong_truck(length2);
	truckTrailer_Left = Translate(-1.2f,0.25f,0.5f) * RotateY(trailerControllerLeft) * Translate(0, 0, (float)-0.25);
	hinhVuong_truckLeft(DoorLeft);
	truckTrailer_Right = Translate(-1.2f,0.25f,-0.5f) * RotateY(trailerControllerRight) * Translate(0, 0, (float)0.25);
	hinhVuong_truckRight(DoorRight);
}
float wheelZ = 0, wheelY = 0;
void Wheels()
{
	bienDoi joint1((float)0.8, (float)-0.3, 0, 90, 0, 0, (float)0.15, (float)0.15, (float)1.1);
	bienDoi joint2((float)-0.5, (float)-0.3, 0, 90, 0, 0, (float)0.15, (float)0.15, (float)1.1);
	bienDoi joint3(-1, (float)-0.3, 0, 90, 0, 0, (float)0.15, (float)0.15, (float)1.1);
	bienDoi wheel1(0, 0, 0, 90, 0, 0, (float)0.25, (float)0.25, (float)0.1);
	bienDoi wheel2(0, 0, 0, 90, 0, 0, (float)0.25, (float)0.25, (float)0.1);
	bienDoi wheel3(0, 0, 0, 90, 0, 0, (float)0.25, (float)0.25, (float)0.1);
	bienDoi wheel4(0, 0, 0, 90, 0, 0, (float)0.25, (float)0.25, (float)0.1);
	bienDoi wheel5(0, 0, 0, 90, 0, 0, (float)0.25, (float)0.25, (float)0.1);
	bienDoi wheel6(0, 0, 0, 90, 0, 0, (float)0.25, (float)0.25, (float)0.1);
	hinhTru_wheelhinge(joint1);
	hinhTru_wheelhinge(joint2);
	hinhTru_wheelhinge(joint3);
	truck *= Translate((float)0, 0, 0) * RotateX(trailerRx) * Scale(1, 1, 1);

	wheels = Translate((float)-0.5, (float)-0.3, (float)-0.5) * RotateZ(wheelZ);
	hinhTru_wheels(wheel3);
	wheels = Translate((float)-0.5, (float)-0.3, (float)0.5) * RotateZ(wheelZ);
	hinhTru_wheels(wheel4);
	wheels = Translate(-1, (float)-0.3, (float)-0.5) * RotateZ(wheelZ);
	hinhTru_wheels(wheel5);
	wheels = Translate(-1, (float)-0.3, (float)0.5) * RotateZ(wheelZ);
	hinhTru_wheels(wheel6);

	wheels = Translate((float)0.8, (float)-0.3, (float)-0.5) * RotateZ(wheelZ) * RotateY(wheelY);
	hinhTru_wheels(wheel1);
	wheels = Translate((float)0.8, (float)-0.3, (float)0.5) * RotateZ(wheelZ) * RotateY(wheelY);
	hinhTru_wheels(wheel2);
}
void Truck()
{
	Chassis();
	Trailer();
	Wheels();
}
// Room
mat4 instance_room, instance_room_left, instance_room_right;
void hinhTron_room(bienDoi hinhTron)
{
	choseColor(155, 68, 68, 100);
	mat4 instance = Translate(hinhTron.translateX, hinhTron.translateY, hinhTron.translateZ) * RotateX(hinhTron.rotateX) * RotateY(hinhTron.rotateY) * RotateZ(hinhTron.rotateZ) * Scale(hinhTron.scaleX, hinhTron.scaleY, hinhTron.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_quat * instance);
	glDrawArrays(GL_TRIANGLES, diemHinhTru + diemHinhLapPhuong, diemHinhTron);
}
void hinhVuong_room(bienDoi hinhVuong)
{
	//rgb(255, 247, 138)rgb(182, 115, 82)

	choseColor(182, 115, 82, 190);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhVuong_room2(bienDoi hinhVuong)
{
	//rgb(155, 68, 68)
	choseColor(155, 68, 68, 190);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhVuong_room3(bienDoi hinhVuong)
{
	//rgb(155, 68, 68)(228, 228, 208)
	
	mat4 instance =  Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY)*  RotateZ(hinhVuong.rotateZ)  * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_room * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhVuong_room4(bienDoi hinhVuong)
{
	//rgb(155, 68, 68)(228, 228, 208)
	choseColor(58, 196, 121,1);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home  * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
void hinhVuong_roomLeft(bienDoi hinhVuong)
{
	choseColor(165, 221, 155, 190);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_room_right * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}

void hinhVuong_roomRight(bienDoi hinhVuong)
{
	choseColor(165, 221, 155,190);
	mat4 instance = Translate(hinhVuong.translateX, hinhVuong.translateY, hinhVuong.translateZ) * RotateX(hinhVuong.rotateX) * RotateY(hinhVuong.rotateY) * RotateZ(hinhVuong.rotateZ) * Scale(hinhVuong.scaleX, hinhVuong.scaleY, hinhVuong.scaleZ);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance_home * instance_room_left * instance);
	glDrawArrays(GL_TRIANGLES, 0, diemHinhLapPhuong);
}
float roomXLeft = 0, roomXRight = 0;
void banner() {
	bienDoi T((float)3.5, (float)5.5, (float)-6, 0, 0, 0,(float) 2.5, (float)0.5, (float)0.5);
	bienDoi T2((float)3.5, (float)3.75, (float)-6, 0, 0, 0, 0.5, (float)3, (float)0.5);
	bienDoi O(4, (float)7, (float)-23, 0, 0, 0, 5, 5, 1);
	bienDoi Y((float)0, (float)0, (float)0, 0, 0, 0, (float)3, (float)0.5, (float)0.5);
	bienDoi Y1((float)0., (float)0, (float)0, 0, 0, 0, (float)1.75, (float).5, (float)0.5);
	//bienDoi Y2(-3, (float)3.5, (float)-6, 0, 0, 0, 0.5, (float)2, (float)0.5);
	hinhTron_room(O);
	// setColor to rô
	
	hinhVuong_room2(T);
	hinhVuong_room2(T2);
//	hinhVuong_room2(Y2);
	/// set Color to room3
	choseColor(155, 68, 68, 190);
	instance_room = Translate((float)-3, (float)2.75, (float)-6) * RotateZ(-60) * Translate(-1.75, 0.25, 0);
	hinhVuong_room3(Y);
	instance_room = Translate((float)-3, (float)5, (float)-6) * RotateZ(40);
	hinhVuong_room3(Y1);
}
void room()
{
	bienDoi bottom2(0, -3.1f, 0, 0, 0, 0, 100, (float)0.1, 100);
	bienDoi bottom(0, -3, 0, 0, 0, 0, 12, (float)0.1, 15);
	bienDoi before(0, (float)-0.5, (float)7.5, 0, 0, 0, 12, 5,(float) 0.1);
	bienDoi Left((float)6, (float)-0.5, 0, 0, 0, 0,(float) 0.1, 5, 15);
	bienDoi Right((float)-6, (float)-0.5, 0, 0, 0, 0,(float) 0.1, 5, 15);
	bienDoi top(0, 2, 0, 0, 0, 0, 12, (float)0.1, 15);
	bienDoi RoomDoor(0, (float)-0.5, (float)-7.55, 0, 0, 0, 5, 5, (float) 0.1);
	bienDoi RoomDoorLeft((float) 1, (float)-1, (float)-7.4, 0, 0, 0, 2,4 ,(float) 0.1);
	bienDoi RoomDoorRight((float)  -1, (float)-1, (float)-7.4, 0, 0, 0,2, 4, (float)0.1);
	bienDoi after((float)4, (float)-0.5, (float)-7.5, 0, 0, 0, 4, 5, (float)0.1);
	bienDoi after1((float)-4, (float)-0.5, (float)-7.5, 0, 0, 0, 4, 5, (float)0.1);
	bienDoi after2((float) 0, (float)1.5, (float)-7.5, 0, 0, 0, 12, 1, (float)0.1);
	bienDoi after3((float)-3, (float)-1, (float)-7.6, 0, 0, 0,(float) 0.1, 4, (float)0.1);
	bienDoi after4((float)3, (float)-1, (float)-7.6, 0, 0, 0, (float) 0.1, 4, (float)0.1);
	bienDoi after5((float)0, (float)1.3, (float)-7.7, 0, 0, 0, (float)6,(float) 0.5, (float)0.1);
	bienDoi Banner((float)0, (float)2, (float)-7.5, 30, 0, 0, 12, 2, (float)0.1);
	//bienDoi led((float)0, (float)1.65, (float)-5, 0, 0, 0, 1, 1, (float)1);
	//instance_room = Translate(0, 0, 0);
	banner();
	
	hinhVuong_room(Left);
	hinhVuong_room(Right);
	hinhVuong_room(bottom);
	hinhVuong_room4(bottom2);
	hinhVuong_room(after);
	hinhVuong_room(after1);
	hinhVuong_room(after2);
	hinhVuong_room2(after3);
	hinhVuong_room2(after4);
	hinhVuong_room2(after5);
	hinhVuong_room(before);
	hinhVuong_room(top);
	//hinhTron_room(led);
	hinhVuong_room(Banner);
	instance_room_left = Translate(roomXLeft, 0, 0);
	hinhVuong_roomLeft(RoomDoorLeft);
	instance_room_right = Translate(roomXRight, 0, 0);
	hinhVuong_roomRight(RoomDoorRight);
}
float homeX = 0, homeY = 0, homeZ = 0;
float truckRX = 0, truckRY = 0, truckRZ = 0;
float truckX = 0, truckY = 0, truckZ = 0;
void Tu()
{
	// Hang cuoi
	instance_tu = Translate(3, -2, 7) * RotateY(180);
	tuControl();
	instance_tu = Translate(-3, -2,7) * RotateY(180);
	tuControl();
	instance_tu = Translate(0, -2, 7) * RotateY(180);
	tuControl();
	// hang doc
	instance_tu = Translate(2, -2, 3) * RotateY(90);
	tuControl();
	instance_tu = Translate(1.5, -2, 3) * RotateY(-90);
	tuControl();
	instance_tu = Translate(1.5, -2, 1) * RotateY(-90);
	tuControl();
	instance_tu = Translate(2, -2, 1) * RotateY(90);
	tuControl();
	instance_tu = Translate(5, -2, 3) * RotateY(-90);
	tuControl();
	instance_tu = Translate(5, -2, 1) * RotateY(-90);
	tuControl();
	// hang doc 2
	tuControl();
	instance_tu = Translate(-2, -2, 3) * RotateY(-90);
	tuControl();
	instance_tu = Translate(-1.5, -2, 3) * RotateY(90);
	tuControl();
	instance_tu = Translate(-1.5, -2, 1) * RotateY(90);
	tuControl();
	instance_tu = Translate(-2, -2, 1) * RotateY(-90);
	tuControl();
	instance_tu = Translate(-5, -2, 3) * RotateY(90);
	tuControl();
	instance_tu = Translate(-5, -2, 1) * RotateY(90);
	tuControl();
}
void quat() {
	instance_quat = Translate(2,(float) 1.5, -2) * Scale((float)0.5, (float)0.5, (float)0.5);
	quatControl();
	instance_quat = Translate(-2, (float)1.5, -2) * Scale((float)0.5, (float)0.5, (float)0.5);
	quatControl();
	instance_quat = Translate(2, (float)1.5, 2) * Scale((float)0.5, (float)0.5, (float)0.5);
	quatControl();
	instance_quat = Translate(-2, (float)1.5, 2) * Scale((float)0.5, (float)0.5, (float)0.5);
	quatControl();
	instance_quat = Translate(2, (float)1.5, 6) * Scale((float)0.5, (float)0.5, (float)0.5);
	quatControl();
	instance_quat = Translate(-2, (float)1.5, 6) * Scale((float)0.5, (float)0.5, (float)0.5);
	quatControl();
}
void List_Truck() {
	float ry = 90;
	float sx = 0.2f, sy = 0.2f, sz = 0.2f;
	float tx = 1.5f, ty = -1.35f;
	truck = Translate(tx, ty, 0.5f) * RotateY(ry) * Scale(sx, sy, sz);
	Truck();
	truck = Translate(tx, ty, 1.5f) * RotateY(ry) * Scale(sx, sy, sz);
	Truck();
	truck = Translate(tx, ty, 3.5f) * RotateY(ry) * Scale(sx, sy, sz);
	Truck();
	truck = Translate(tx, ty, 2.5f) * RotateY(ry) * Scale(sx, sy, sz);
	Truck();
	truck = Translate(tx, ty, 0.5f) * RotateY(ry) * Scale(sx, sy, sz);
	Truck();
	truck = Translate(tx, ty, 1.5f) * RotateY(ry) * Scale(sx, sy, sz);
	Truck();
	truck = Translate(tx, ty, 3.5f) * RotateY(ry) * Scale(sx, sy, sz);
	Truck();
	truck = Translate(tx, ty, 2.5f) * RotateY(ry) * Scale(sx, sy, sz);
	Truck();
}
/// END DAOXUANDONG

/// =============== DO VAN HUNG

mat4 instance_maytang;
mat4 model_xt;

GLfloat xx = 0, yy = 0, zz = 0;
mat4 model_xetang;
mat4 mv;
void xetang(GLfloat x, GLfloat y, GLfloat z) {
	instance_maytang = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * model_xt * model_xetang * instance_maytang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
//cau tao xe tang
GLfloat xe_tang =(float) 0.1;
GLfloat xetang1 =(float) xe_tang * 1.2f;
GLfloat xetang2 =(float) xe_tang * 4;
GLfloat dd_vatlieu =(float) xe_tang * 0.1f;
//than
GLfloat than_xt1 =(float) xe_tang * 2;
GLfloat than_xt2 =(float) than_xt1 * 4;
GLfloat than_xt3 =(float) than_xt1 * 1.5f;

//banh xe
GLfloat banh_xt1 =(float) xe_tang * 1.5f;
GLfloat banh_xt2 =(float) xe_tang * 5;
GLfloat banh_xt3 =(float) xe_tang * 1.5f;
GLfloat theta[] = { 0, 0, 0 ,0 };

void Dau_xetang() {
	mat4 dau;

	ambient = setColor(238, 154, 73);
	diffuse = setColor(238, 154, 73);
	specular = setColor(238, 154, 73);
	Color(ambient, diffuse, specular, 190);

	model_xetang = dau * Translate(than_xt1 * 0.75f, than_xt3 * 0.5f / 2, -than_xt2 / 2); // than
	xetang(than_xt1 * 2.5f, than_xt3, than_xt2);
	model_xetang = dau * Translate(than_xt1 * 0.7f, than_xt3 * 0.7f, -than_xt2 / 1.3f);
	xetang(than_xt1 * 2, than_xt3 * 0.5f, than_xt2 * 0.5f / 2);
}
void BanhxeTrai() {
	mat4 banh;
	ambient = setColor(139, 69, 19);
	diffuse = setColor(139, 69, 19);
	specular = setColor(139, 69, 19);
	Color(ambient, diffuse, specular, 190);


	model_xetang = banh * Translate(0, 0, 0);
	xetang(banh_xt1, banh_xt3, banh_xt2 * 2);
	model_xetang = banh * Translate(0, banh_xt3 / 2, 0);
	xetang(banh_xt1, banh_xt3, banh_xt2 * 1.5f);
	model_xetang = banh * Translate(0, -banh_xt3 / 2, 0);
	xetang(banh_xt1, banh_xt3, banh_xt2 * 1.5f);
	model_xetang = banh * Translate(0, banh_xt1 / 2 - banh_xt3 / 2 - banh_xt3 / 4, banh_xt2 - xetang2 * 0.25f) * RotateX(55);
	xetang(banh_xt1, banh_xt3, banh_xt1);
	model_xetang = banh * Translate(0, -banh_xt1 / 2 + banh_xt3 / 2 + banh_xt3 / 4, banh_xt2 - xetang2 * 0.25f) * RotateX(125);
	xetang(banh_xt1, banh_xt3, banh_xt1);
	model_xetang = banh * Translate(0, banh_xt1 / 2 - banh_xt3 / 2 - banh_xt3 / 4, -banh_xt2 + xetang2 * 0.25f) * RotateX(125);
	xetang(banh_xt1, banh_xt3, banh_xt1);
	model_xetang = banh * Translate(0, -banh_xt1 / 2 + banh_xt3 / 2 + banh_xt3 / 4, -banh_xt2 + xetang2 * 0.25f) * RotateX(55);
	xetang(banh_xt1, banh_xt3, banh_xt1);
}
void BanhxePhai() {
	mat4 banh;
	ambient = setColor(139, 69, 19);
	diffuse = setColor(139, 69, 19);
	specular = setColor(139, 69, 19);
	Color(ambient, diffuse, specular, 190);

	model_xetang = banh * Translate(0, 0, 0);
	xetang(banh_xt1, banh_xt3, banh_xt2 * 2);
	model_xetang = banh * Translate(0, banh_xt3 / 2, 0);
	xetang(banh_xt1, banh_xt3, banh_xt2 * 1.5f);
	model_xetang = banh * Translate(0, -banh_xt3 / 2, 0);
	xetang(banh_xt1, banh_xt3, banh_xt2 * 1.5f);
	model_xetang = banh * Translate(0, banh_xt1 / 2 - banh_xt3 / 2 - banh_xt3 / 4, banh_xt2 - xetang2 * 0.25f) * RotateX(55);
	xetang(banh_xt1, banh_xt3, banh_xt1);
	model_xetang = banh * Translate(0, -banh_xt1 / 2 + banh_xt3 / 2 + banh_xt3 / 4, banh_xt2 - xetang2 * 0.25f) * RotateX(125);
	xetang(banh_xt1, banh_xt3, banh_xt1);
	model_xetang = banh * Translate(0, banh_xt1 / 2 - banh_xt3 / 2 - banh_xt3 / 4, -banh_xt2 + xetang2 * 0.25f) * RotateX(125);
	xetang(banh_xt1, banh_xt3, banh_xt1);
	model_xetang = banh * Translate(0, -banh_xt1 / 2 + banh_xt3 / 2 + banh_xt3 / 4, -banh_xt2 + xetang2 * 0.25f) * RotateX(55);
	xetang(banh_xt1, banh_xt3, banh_xt1);
}
void Be_sung() {
	mat4 tay;
	ambient = setColor(205, 133, 63);
	diffuse = setColor(205, 133, 63);
	specular = setColor(205, 133, 63);
	Color(ambient, diffuse, specular, 190);
	model_xetang = tay * Translate(-than_xt1 * 1.65f, than_xt3 * 0.5f, than_xt2 * 0.3f);
	xetang(than_xt1 * 2, than_xt3 * 0.8f, than_xt2 * 0.4f);
	model_xetang = tay * Translate(-than_xt1 * 1.65f, than_xt3 * 0.5f, than_xt2 * 0.3f);//* RotateY(theta[0])
	xetang(than_xt1 * 1.4f, than_xt3 * 1.2f, than_xt2 * 0.35f);
}

void Ke_sung() {
	mat4 longsung;
	choseColor(244, 164, 96, 190);
	model_xetang = longsung * Translate(-than_xt1 * 1.65f, than_xt3 * 1.2f, than_xt2 * 0.3f) * RotateY(theta[0]);
	for (float i = 0; i < 360; i++) {
		longsung = RotateY(i) * Scale(than_xt1 / 2, than_xt3 * 0.5f, than_xt2 * 0.3f);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * model_xt * model_xetang * longsung);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void Long_sung() {
	mat4 longsung;
	choseColor(244, 164, 96, 190);
	model_xetang = longsung * Translate(-than_xt1 * 1.65f, than_xt3 * 1.4f, than_xt2 * 0.4f) * RotateY(theta[0]) * Translate(0, 0, than_xt2 * 0.4f) * RotateX(theta[1]);
	for (float i = 0; i < 360; i++) {
		longsung = RotateZ(i) * Scale(than_xt1 * 0.2f, than_xt3 * 0.1f, than_xt2 * 0.8f);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * model_xt * model_xetang * longsung);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
GLfloat xd = 0, yd = 0, zd = 0;
void Dan() {
	mat4 dan;	
	choseColor(244, 164, 96, 190);
	model_xetang = dan * Translate(-than_xt1 * 1.65f, than_xt3 * 1.4f, than_xt2 * 0.4f) * RotateY(theta[0]) * Translate(0, 0, than_xt2 * 0.4f) * RotateX(theta[1]) * Translate(xd, yd, zd);
	for (float i = 0; i < 360; i++) {
		dan = RotateZ(i) * Scale(than_xt1 * 0.1f, than_xt3 * 0.05f, than_xt2 * 0.1f);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, mv * model_xt * model_xetang * dan);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void Sung() {
	Be_sung();
	Ke_sung();
	Long_sung();
	Dan();
}
GLfloat xx1 = 0, yy1 = 0, zz1 = 0;
void Than_xe_tang() {
	model_xt = Translate(xx1, yy1, zz1) * RotateX(xx) * RotateY(yy) * RotateZ(zz);
	Dau_xetang();
	model_xt *= Translate(-than_xt1 / 2 - banh_xt1 / 2, 0, -than_xt2 * 0.5f);
	BanhxeTrai();
	model_xt *= Translate(than_xt1 * 5 / 2 + banh_xt1, 0, 0);
	BanhxePhai();

}
void Xe_tang() {
	Than_xe_tang();
	Sung();
}
void List_XeTank() {

	mv = Translate((float)0.7, (float)-1.9, 7) * RotateY(120) * Scale((float)0.6, (float)0.6, (float)0.6);
	Xe_tang();
	mv = Translate((float)-0.4, (float)-1.9,(float) 7.1) * RotateY(120) * Scale((float)0.6, (float)0.6, (float)0.6);
	Xe_tang();
	mv = Translate((float)0.7, (float)-1.4, (float)7.1) * RotateY(120) * Scale((float)0.6, (float)0.6, (float)0.6);
	Xe_tang();
	mv = Translate((float)-0.4, (float)-1.4, (float)7.1) * RotateY(120) * Scale((float)0.6, (float)0.6, (float)0.6);
	Xe_tang();
}
//Bàn thu ngân
GLfloat banf = 0.1f;
GLfloat matban1 = banf * 2;
GLfloat matban2 = banf * 4;
GLfloat matban3 = banf * 1.5f;
GLfloat maytinh = banf * 4;
mat4 instance_ban, model_b, model_ban;

void banthungan(GLfloat x, GLfloat y, GLfloat z) {
	instance_ban = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_b * model_ban * instance_ban);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void thanban() {
	mat4 tb;
	choseColor(255, 228, 196, 190);
	model_ban = tb * Translate(0, 0, 0);
	banthungan(matban1 * 3.5f, matban2, matban3 / 2);
	model_ban = tb * Translate(matban1 * 1.55f, -matban2 * 0.02f / 2, matban3 * 1.15f);
	banthungan(matban3 / 2, matban2, matban1 * 2);
	choseColor(139, 71, 38, 190);
	model_ban = tb * Translate(-matban1 / 2 * 0.4f, matban2 / 2 * 0.8f, matban3 * 0.8f); //mat ban 
	banthungan(matban1 * 3.1f, matban2 * 0.02f, matban3 * 1.2f);
	model_ban = tb * Translate(matban1 * 0.9f, matban2 / 2 * 0.8f, matban3 * 1.9f); //mat ban 
	banthungan(matban3 * 1.2f, matban2 * 0.02f, matban1 * 0.8f);
	choseColor(255, 228, 196, 190);
	model_ban = tb * Translate(-matban1 * 1.55f, -matban2 * 0.1f / 2, matban3 * 0.8f); // chan
	banthungan(matban3 / 2, matban2 * 0.9f, matban3 * 1.2f);

}
void May_tinh() {
	mat4 mt;
	choseColor(0,0,0, 190);
	model_ban = mt * Translate(0, maytinh * 0.8f, 0.05f) * RotateX(-10); //màn hình
	banthungan(maytinh * 0.5f, maytinh * 0.5f, maytinh * 0.02f);
	choseColor(71,118,255, 190);
	model_ban = mt * Translate(0, maytinh * 0.8f, 0.055f) * RotateX(-10); //màn hình
	banthungan(maytinh * 0.4f, maytinh * 0.4f, maytinh * 0.02f);
	banthungan(maytinh * 0.5f, maytinh * 0.5f, maytinh * 0.02f);
	choseColor(0,0,0, 190);
	model_ban = mt * Translate(0, maytinh * 0.55f, 0.05f); // đế màn hình
	banthungan(maytinh * 0.1f, maytinh * 0.3f, maytinh * 0.01f);
	choseColor(0, 0, 0, 190);
	model_ban = mt * Translate(0, maytinh * 0.4f, 0.1f);
	banthungan(maytinh * 0.4f, maytinh * 0.05f, maytinh * 0.15f);

}
GLfloat y_thungan;
void Ban_thu_ngan() {
	//	model_xt = Translate(xx1, yy1, zz1) * RotateX(xx) * RotateY(yy) * RotateZ(zz);
	model_b = Translate((float)-2.5f, (float)-2, -5) * RotateY(-90) * Scale(4, 4, 4)*RotateY(y_thungan);
	thanban();
	
	May_tinh();
}


// ================== END DVH


void home()
{
	instance_home = RotateX(homeX) * RotateY(homeY) * RotateZ(homeZ);
	quat();
	Tu();
	List_Truck();
	List_XeTank(); 
	Ban_thu_ngan();

	
	room();
}
GLfloat zNear = (float)0.03, zFar = 10;
int phi = 0;
float yEye = 0;
float xEye = 0, yAt = 0, xAt = 0, gocQuayNgangCam = 0, gocQuayDocCam = 0;
float QuayCamera[3] = {90, 0, 0};
float eyeX = 0, eyeY = 0, eyeZ = 0, zEye = 0;
float ryEye;

vec4 eye(0, -1, -20, 1), at(0, 0, 0, 1), up(0, 1, 0, 1);
GLfloat l = (float)-0.01, r =(float) 0.01, bottom =(float) -0.01, t =(float) 0.01;
mat4 view_mat;
mat4 move_mat;
GLfloat matXY[2]; // doc ngang
vec4 mat_dinhhuong(0, 0, 1, 1);
void Eye()
{
	// -------------------------Tạo mắt nhìn--------------------------
	vec4 at = eye + normalize(mat_dinhhuong);
	eye.w = at.w = 1;
	vec4 up(0, 1, 0, 1);

	mat4 view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	// -------------------------Tạo khung nhìn--------------------------
	mat4 projection = Frustum(l, r, bottom, t, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const vec3 viewer_pos = (eye.x, eye.y,eye.z); /*Trùng với eye của camera*/

	Eye();
	home();
	//Phong();

	glutSwapBuffers();
}
bool CAMERA = true;
bool DAOXUANDONG = false, DOVANHUNG = false, NGUYENVANHUY = false;
GLfloat speedTranslate = (float)0.3;
GLfloat speedRotate = 5;
float truckRotate = 0, truckTran = 0;
bool SPEED[3];
void Idle_Quat() {
	if (SPEED[0] == true) {
		quatController.ry += 0;
	}
	else if (SPEED[1] == true) {
		quatController.ry +=(float) 1;
	}
	else if (SPEED[2] == true) {
		quatController.ry += (float)10;
	}
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler
	
	if (CAMERA == true) {
		switch (key) {
		case 033:			// 033 is Escape key octal value
			exit(1);		// quit program
			break;
		case '9':
			CAMERA = false;
			DAOXUANDONG = true;
			DOVANHUNG = false;
			NGUYENVANHUY = false;
			break;
		case '8':
			CAMERA = false;
			DOVANHUNG = true;
			DAOXUANDONG = false;
			NGUYENVANHUY = false;
			break;
		case '7':
			CAMERA = false;
			DOVANHUNG = false;
			DAOXUANDONG = false;
			NGUYENVANHUY = true;
			break;
		case '1':
			light_ambient = setLight(10, 10, 10);
			light_diffuse = setLight(55, 55, 55);
			light_specular = setLight(55, 55, 55);
			glClearColor(53/255, 55/255, 75/255,1.0);
			glutPostRedisplay();
			break;
		case '2':
			//rgb(102, 90, 72)
			light_ambient = setLight(51,51,51);
			light_diffuse = setLight(255, 255, 255);
			light_specular = setLight(255, 255, 255);
			glClearColor(255 / 255, 255 / 255, 255 / 255, 1.0);
			glutPostRedisplay();
			break;
		case '3':
			light_position = vec4(0, 4, 1, 1.0);
			glutPostRedisplay();
			break;
		case '4':
			light_position = vec4(20, 50, 1, 1.0);
			break;
			// Chỉnh tọa độ của mắt
		case 'w':
			eye = eye + RotateY(matXY[1]) * vec4(0.0, 0.0, speedTranslate, 1);
			break;
		case 's':
			eye = eye - RotateY(matXY[1]) * vec4(0.0, 0.0, speedTranslate, 1);
			break;
		case 'd':
			eye = eye - RotateY(matXY[1]) * vec4(speedTranslate, 0.0, 0.0, 1);
			break;
		case 'a':
			eye = eye + RotateY(matXY[1]) * vec4(speedTranslate, 0.0, 0.0, 1);
			break;
		case 'q':
			eye = eye - RotateY(matXY[1]) * vec4(0.0, speedTranslate, 0.0, 1);
			break;
		case 'e':
			eye = eye + RotateY(matXY[1]) * vec4(0.0, speedTranslate, 0.0, 1);
			break;
		case 'l':
			matXY[1] -= speedRotate;
			if (matXY[1] >= 360) matXY[1] -= 360;
			if (matXY[1] < 0) matXY[1] += 360;
			break;
		case 'j':
			matXY[1] += speedRotate;
			if (matXY[1] >= 360) matXY[1] -= 360;
			if (matXY[1] < 0) matXY[1] += 360;
			break;
		case 'i':
			matXY[0] -= speedRotate;
			if (matXY[0] < -75) {
				matXY[0] = -75;
			}
			break;
		case 'k':
			matXY[0] += speedRotate;
			if (matXY[0] > 75) {
				matXY[0] = 75;
			}
			break;

		}

		mat_dinhhuong = RotateY(matXY[1]) * RotateX(matXY[0]) * vec4(0, 0, 1, 1);
		mat_dinhhuong.w = 1;


		glutPostRedisplay();
	}
	if (DAOXUANDONG == true) {
			switch (key)
			{
			case 033:	 // 033 is Escape key octal value
				exit(1); // quit program
				break;

			case '0':
				CAMERA = true;
				DAOXUANDONG = false;
				break;
			case 'c':
				roomXLeft -= (float)0.1;
				if (roomXLeft <= -2)  roomXLeft = -2;
				roomXRight += (float)0.1;
				if (roomXRight >= 2)  roomXRight = 2;
				glutPostRedisplay();
				break;
			case 'C':
				roomXLeft += (float)0.1;
				if (roomXLeft >= 0)  roomXLeft = 0;
				roomXRight -= (float)0.1;
				if (roomXRight <= 0)  roomXRight = 0;
				glutPostRedisplay();
				glutPostRedisplay();
				break;
			case 'm':
				truckTran += 0.1f;
				instance_truck =  Translate(truckTran, 0, 0) * RotateY(truckRotate);
				glutPostRedisplay();
				break;
			case 'M':
				truckTran -= 0.1f;
				instance_truck = Translate(truckTran, 0, 0) * RotateY(truckRotate);
				glutPostRedisplay();
				
				break;
			case 'n':
				truckRotate += 10.0f;
				instance_truck = Translate(truckTran,0,0) * RotateY(truckRotate) * Translate(0, 0, 1);
				glutPostRedisplay();
				break;
			case ',':
				truckRotate = 0;
				truckTran = 0;
				instance_truck = Translate(truckTran, 0, 0) * RotateY(truckRotate) ;
				glutPostRedisplay();
			case '1':
				SPEED[0] = false;
				SPEED[1] = true;
				SPEED[2] = false;
				glutIdleFunc(Idle_Quat);

				break;
			case '2':
				SPEED[0] = false;
				SPEED[1] = false;
				SPEED[2] = true;
				glutIdleFunc(Idle_Quat);
				break;
			case '3':
				SPEED[0] = true;
				SPEED[1] = false;
				SPEED[2] = false;
				glutIdleFunc(Idle_Quat);
				break;
			case 'v':
				trailerControllerLeft += 10;
				trailerControllerRight -= 10;
				if (trailerControllerLeft >= 120)
					trailerControllerLeft = 120;
				if (trailerControllerRight <= -120)
					trailerControllerRight = -120;

				glutPostRedisplay();
				break;
			case 'V':
				trailerControllerLeft -= 10;
				trailerControllerRight += 10;
				if (trailerControllerLeft <= 0)
					trailerControllerLeft = 0;
				if (trailerControllerRight >= 0)
					trailerControllerRight = 0;

				glutPostRedisplay();
				break;
			case 'x':
				homeX += 10;
				glutPostRedisplay();
				break;
			case 'X':
				model *= RotateX(-dr);
				glutPostRedisplay();
				break;
			case 'y':
				homeY += 10;
				glutPostRedisplay();
				break;
			case 'Y':
				model *= RotateY(-dr);
				glutPostRedisplay();
				break;
			case 'z':
				homeZ += 10;
				glutPostRedisplay();
				break;
			case 'Z':
				model *= RotateZ(-dr);
				glutPostRedisplay();
				break;
			//case 'n':
			//	truckX += (float)0.1;
			//	//wheelZ -= 10;
			//	glutPostRedisplay();
			//	break;
			//case 'N':
			//	truckX -= (float)0.1;
			//	//wheelZ += 10;
			//	glutPostRedisplay();
			//	break;

			}
	}
	if (DOVANHUNG == true) {
		switch (key) {
		case '0':
			CAMERA = true;
			DOVANHUNG = false;
			break;
		case 'y':
			yy += 10;
			glutPostRedisplay();
			break;
		case 'Y':
			y_thungan += 10;
			glutPostRedisplay();
			break;

		case 'w':
			zz1 += 0.1f;
			glutPostRedisplay();
			break;
		case 's':
			zz1 -= 0.1f;
			glutPostRedisplay();
			break;
		case 'r':
			theta[0] += 5;
			if (theta[0] > 360) theta[0] -= 360;
			glutPostRedisplay();
			break;
		case 'f':
			theta[1] -= 5;
			if (theta[1] <= -15) theta[1] = -15;
			glutPostRedisplay();
			break;
		case 'F':
			theta[1] += 5;
			if (theta[1] >= 0) theta[1] = 0;
			glutPostRedisplay();
			break;

		case 'b':
			zd += 1;
			if (zd >= 1) zd = 1;
			glutPostRedisplay();
			break;
		case 'B':
			zd = 0;

			glutPostRedisplay();
			break;
		}
	}
}
void keyboard1(unsigned char key, int x, int y)
{
	// keyboard handler


}
int lastX = 0, lastY = 0;

void mouseMove(int x, int y)
{
	int dx = x - lastX;
	int dy = y - lastY;

	// Cập nhật lastX và lastY cho lần di chuyển tiếp theo
	lastX = x;
	lastY = y;

	// Bây giờ, dx và dy giữ giá trị của sự di chuyển chuột từ khung hình cuối cùng.
	// Bạn có thể sử dụng các giá trị này để di chuyển một đối tượng trong cảnh của bạn tương ứng.
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_ENTERED)
	{
		// Chuột trái được nhấn
		lastX = x;
		lastY = y;
		glutMotionFunc(mouseMove);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// Chuột trái được thả
		glutMotionFunc(NULL);
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		 gocQuayNgangCam += 1;
		lastX = x;
		lastY = y;
		glutMotionFunc(mouseMove);
		//		glutPostRedisplay();
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		glutMotionFunc(NULL);
	}

}
void wheelMouse(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		zNear *= (float)1.1;
		zFar *= (float) 1.1;
		glutPostRedisplay();
	}
	else
	{
		zNear *=(float) 0.9;
		zFar *= (float) 1.6;
		glutPostRedisplay();
	}
	/*case 'w': zNear *= 1.1; zFar *= 1.1; glutPostRedisplay(); break;
case 's': zNear *= 0.9; zFar *= 1.6; glutPostRedisplay(); break;*/
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("A Cube is rotated by keyboard and shaded");

	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	// glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMouseFunc(mouse);
	glutMouseWheelFunc(wheelMouse);
	glutMainLoop();
	return 0;
}
