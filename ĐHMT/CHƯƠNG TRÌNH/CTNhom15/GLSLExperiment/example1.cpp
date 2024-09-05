//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/


// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );


typedef vec4 point4;
typedef vec4 color4;
using namespace std;
const int numpointsnon = 18;//so dinh cua non
const int numpointschop = 36;
const int numpointslp = 36;
const int NumPoints = numpointsnon + numpointschop + numpointslp;
const int firstpointnon = 0;
const int firstpointchop = firstpointnon + numpointsnon;
const int firstpointlp = firstpointchop + numpointslp;
// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints];


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;
GLuint model_loc;
GLuint projection_loc, colors_loc, view_loc;

mat4 instance, instance_canh, model, ttcanh, ttthan, instance_than;
mat4 h2_instance;
mat4 h2_instance_hop;
mat4 h2_model;


GLfloat spincanhquat, gx, gy, gz = 0.0f;

GLfloat anpha;
GLfloat tv_open = 0.0;
GLfloat beta;
mat4 instance_h_tt_tuong, h_tt_bpcua, h_tt_cua;
GLfloat cuay = 0.0f;
GLfloat  remy = 0.01f;

GLfloat theta = 0.0f;
GLfloat alphaa = 0.0f;
GLfloat ap = 0.0f;
GLfloat ax = 0.0f;
mat4 k_dichuyen, k_ttghe, k_ttban, k_ttchan, k_ttcua, k_dichuyenb, k_dichuyeng;

void initNon() {
	vertices[0] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[1] = point4(0.5, -0.5, -0.5, 1.0);
	vertices[2] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[3] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(0, 0, 0, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 0.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 0.8, 0.0, 1.0); // orange
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
void initchopcut() {
	vertices[0] = point4(-0.25, 0.25, -0.25, 1.0);
	vertices[1] = point4(0.25, 0.25, -0.25, 1.0);
	vertices[2] = point4(0.25, 0.25, 0.25, 1.0);
	vertices[3] = point4(-0.25, 0.25, 0.25, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(0.5, -0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[7] = point4(-0.5, -0.5, 0.5, 1.0);

	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 0.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 0.8, 0.0, 1.0); // orange
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan

}

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
	vertex_colors[1] = color4(0.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(0.0, 0.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 0.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 0.0, 1.0); // blue
	vertex_colors[5] = color4(0.0, 0.0, 0.0, 1.0); // magenta
	vertex_colors[6] = color4(0.0, 0.0, 0.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 0.0, 0.0, 1.0); // cyan
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

void quad(int a, int b, int c) {
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
}
void makeColorNon(void) {
	quad(1, 0, 3, 2);
	quad(4, 3, 2);
	quad(4, 2, 1);
	quad(4, 1, 0);
	quad(4, 0, 3);
	cout << " Non = " << Index << " points \n";
}
void makeColorChop() {
	cout << " Chop = " << Index << " - ";
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
	cout << Index << " points \n";
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
void generateGeometry( void )
{
	initNon();
	makeColorNon();
	initchopcut();
	makeColorChop();
	initCube();
	makeColorCube();
}


void initGPUBuffers( void )
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

/* Khởi tạo các tham số chiếu sáng - tô bóng*/
point4 light_position(0.0, 0.0, 1.0, 0.0);
color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);

color4 material_ambient(1.0, 1.0, 1.0, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1.0, 0.8, 0.0, 1.0);
float material_shininess = 100.0;

color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;

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


	


	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	model_loc = glGetUniformLocation(program, "Model");
	view_loc = glGetUniformLocation(program, "View");
	projection_loc = glGetUniformLocation(program, "Projection");
	colors_loc = glGetUniformLocation(program, "Colors");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}
void setColor(color4 color) {
	material_diffuse = color;
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
}


//void initGL() {
//	glClearColor(0, 0.4, 0, 0);
//	//glOrtho(-3, 3, -3, 3, -1, 1);
//}

//Ve ban
 //mat4 k_edit;//
void ghe(GLfloat x, GLfloat y, GLfloat z, GLfloat rc, GLfloat gc, GLfloat bc) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, TRUE, model * k_dichuyen  *k_dichuyeng * k_ttghe * instance);
	glUniformMatrix4fv(colors_loc, 1, TRUE, Translate(rc / 255, gc / 255, bc / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}

void ban(GLfloat x, GLfloat y, GLfloat z, GLfloat rc, GLfloat gc, GLfloat bc) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, TRUE, model * k_dichuyen * k_dichuyenb * k_ttban * instance);
	glUniformMatrix4fv(colors_loc, 1, TRUE, Translate(rc / 255, gc / 255, bc / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}

void cua1(GLfloat x, GLfloat y, GLfloat z, GLfloat rc, GLfloat gc, GLfloat bc) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, TRUE, model * k_dichuyen * k_dichuyenb * k_ttcua * Translate(0, 0, z / 2.0) * RotateY(alphaa) * Translate(0, 0, -z / 2.0) * instance);
	glUniformMatrix4fv(colors_loc, 1, TRUE, Translate(rc / 255, gc / 255, bc / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}

void cua2(GLfloat x, GLfloat y, GLfloat z, GLfloat rc, GLfloat gc, GLfloat bc) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, TRUE, model * k_dichuyen * k_dichuyenb * k_ttcua * Translate(ax, 0, 0) * instance);
	glUniformMatrix4fv(colors_loc, 1, TRUE, Translate(rc / 255, gc / 255, bc / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}

void chan(GLfloat x, GLfloat y, GLfloat rc, GLfloat gc, GLfloat bc) {
	instance = Scale(x, y, x);
	glUniformMatrix4fv(model_loc, 1, TRUE, model * k_dichuyen * k_dichuyenb * k_ttchan * instance);
	glUniformMatrix4fv(colors_loc, 1, TRUE, Translate(rc / 255, gc / 255, bc / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}


void sofa_dai() {
	k_ttghe = identity();
	k_ttghe = Translate(0.05, 0.05, -0.05);
	ghe(0.15, 0.1, 0.6, 255, 222, 173);
	k_ttghe = Translate(-0.05, 0.1, -0.05);
	ghe(0.05, 0.2, 0.7, 205, 179, 139);
	k_ttghe = Translate(0.05, 0.075, 0.325 - 0.05);
	ghe(0.15, 0.15, 0.05, 238, 207, 161);
	k_ttghe = Translate(0.05, 0.075, -0.325 - 0.05);
	ghe(0.15, 0.15, 0.05, 238, 207, 161);

}

void sofa_nho() {
	k_ttghe = identity();
	k_ttghe = Translate(0.275, 0.05, 0.2 - 0.05);
	ghe(0.3, 0.1, 0.2, 210, 210, 210);
}

void sofa_vua() {
	k_ttghe = identity();

	k_ttghe = Translate(0.75, 0.05, -0.14);
	ghe(0.15, 0.1, 0.4, 150, 150, 150);
	k_ttghe = Translate(0.85, 0.1, -0.14);
	ghe(0.05, 0.2, 0.5, 90, 90, 90);
	k_ttghe = Translate(0.75, 0.075, -0.365);
	ghe(0.15, 0.15, 0.05, 120, 120, 120);
	k_ttghe = Translate(0.75, 0.075, 0.085);
	ghe(0.15, 0.15, 0.05, 120, 120, 120);
}

void banpk() {
	k_ttban = identity();
	k_dichuyenb = Translate(-0.15, 0, -0.45);
	k_ttban = Translate(0, 0.005, 0);//duoi
	ban(0.3, 0.01, 0.4, 100, 100, 100);
	k_ttban = Translate(0, 0.115, 0);//tren
	ban(0.3, 0.01, 0.4, 200, 200, 200);
	k_ttban = Translate(0, 0.06, 0.195);//tu 1
	ban(0.3, 0.1, 0.01, 150, 150, 150);
	k_ttban = Translate(0, 0.06, 0.055);
	ban(0.3, 0.1, 0.01, 150, 150, 150);
	k_ttban = Translate(-0.145, 0.06, 0.125);
	ban(0.01, 0.1, 0.13, 150, 150, 150);
	k_ttcua = Translate(0.145, 0.06, 0.125);
	cua1(0.01, 0.1, 0.15, 200, 200, 200);
	k_ttban = Translate(0, 0.03, -0.195);//tu 2
	ban(0.3, 0.06, 0.01, 150, 150, 150);
	k_ttban = Translate(0, 0.03, -0.055);
	ban(0.3, 0.06, 0.01, 150, 150, 150);
	k_ttban = Translate(-0.145, 0.03, -0.125);
	ban(0.01, 0.06, 0.13, 150, 150, 150);
	k_ttban = Translate(0, 0.065, -0.125);
	ban(0.3, 0.01, 0.15, 150, 150, 150);
	k_ttcua = Translate(0.145, 0.04, -0.125);
	cua2(0.01, 0.06, 0.15, 200, 200, 200);
	k_ttcua = Translate(0, 0.04, -0.195);
	cua2(0.28, 0.06, 0.01, 150, 150, 150);
	k_ttcua = Translate(0, 0.04, -0.055);
	cua2(0.28, 0.06, 0.01, 150, 150, 150);
	k_ttcua = Translate(0, 0.016, -0.125);
	cua2(0.28, 0.01, 0.13, 180, 180, 180);


}
void bobanhoanchinh() {
	k_dichuyeng = Translate(-0.525, 0, -0.25);
	k_dichuyen = Translate(-0.03, -0.5, 0);
	sofa_dai();
	sofa_nho();
	sofa_vua();
	banpk();
}

//Vẽ khung nhà

void canhquat(GLfloat x, GLfloat y, GLfloat z, GLfloat ax, GLfloat bx, GLfloat cx) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, TRUE, model * ttthan * ttcanh * RotateX(90) * RotateZ(spincanhquat) * instance_canh * instance);
	glUniformMatrix4fv(colors_loc, 1, true, Translate(ax / 255, bx / 255, cx / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}


void than(GLfloat x, GLfloat y, GLfloat z, GLfloat ax, GLfloat bx, GLfloat cx) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, TRUE, model * ttthan * instance_than * instance);
	glUniformMatrix4fv(colors_loc, 1, true, Translate(ax / 255, bx / 255, cx / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}
void bocanhquat()
{	//canh1
	instance_canh = Translate(0, 0.1, 0);
	canhquat(0.04, 0.2, 0.002, 102, 255, 255);
	//canh2
	instance_canh = RotateZ(120) * Translate(0, 0.1, 0);
	canhquat(0.04, 0.2, 0.002, 102, 255, 255);
	//canh2
	instance_canh = RotateZ(-120) * Translate(0, 0.1, 0);
	canhquat(0.04, 0.2, 0.002, 102, 255, 255);
}
void bothan() {
	//thandoc
	instance_than = identity();
	than(0.01, 0.1, 0.01, 0, 230, 235);
}
void quattran() {
	//ttthan = Translate(0, 0.5, 0);
	bothan();
	ttcanh = Translate(0, -0.05 - 0.001, 0);
	bocanhquat();
}

void tuong(GLfloat x, GLfloat y, GLfloat z, GLfloat ax, GLfloat bx, GLfloat cx) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, true, model * instance_h_tt_tuong * instance);
	glUniformMatrix4fv(colors_loc, 1, true, Translate(ax / 255, bx / 255, cx / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}
void bpcua(GLfloat x, GLfloat y, GLfloat z, GLfloat ax, GLfloat bx, GLfloat cx) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, true, model * h_tt_bpcua * instance_h_tt_tuong * instance);
	glUniformMatrix4fv(colors_loc, 1, true, Translate(ax / 255, bx / 255, cx / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}
void canhcua(GLfloat x, GLfloat y, GLfloat z, GLfloat ax, GLfloat bx, GLfloat cx) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, true, model * h_tt_bpcua * h_tt_cua * RotateY(cuay) * Translate(-x / 2, 0, 0) * instance);
	glUniformMatrix4fv(colors_loc, 1, true, Translate(ax / 255, bx / 255, cx / 255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}
void remcuaso() {
	//khungrem
	instance_h_tt_tuong = Translate(-0.6 + 0.65 / 2 - 0.017, 0.1 + 0.01, -0.65 + 0.005);
	tuong(0.65, 0.02, 0.01, 163, 100, 40);
	//rem
	instance_h_tt_tuong = Translate(-0.6 + remy / 2 - 0.017, 0.1 + 0.01 - 0.44 / 2, -0.65 + 0.005 + 0.001 / 2);
	tuong(remy, 0.44, 0.001, 137, 137, 137);
}
void khungcua() {
	//khung trai
	instance_h_tt_tuong = identity();
	bpcua(0.017, 0.7, 0.03, 165, 42, 42);
	//khungphai
	instance_h_tt_tuong = Translate(0.3 - 0.017, 0, 0);
	bpcua(0.017, 0.7, 0.03, 165, 42, 42);
	//khungtren
	instance_h_tt_tuong = Translate(0.15 - 0.017 / 2, 0.7 / 2 - 0.017 / 2, 0);
	bpcua(0.3, 0.017, 0.03, 165, 42, 42);
	//khungduoi
	instance_h_tt_tuong = Translate(0.3 / 2 - 0.017 / 2, -0.7 / 2 + 0.017 / 2, 0);
	bpcua(0.3, 0.017, 0.03, 165, 42, 42);
	//canhcua
	h_tt_cua = Translate(-0.017 / 2 + 0.283, 0, 0);
	canhcua(0.266, 0.666, 0.03, 160, 40, 40);

}
void khungcuakinh() {
	//khung cua x=0.616,y=0.4,z=0.02
	//phan tren
	instance_h_tt_tuong = Translate(-0.6 + 0.616 / 2, 0.1 - 0.01 / 2, -0.7 + 0.02);
	tuong(0.616, 0.01, 0.02, 165, 42, 42);
	//phan duoi
	instance_h_tt_tuong = Translate(-0.6 + 0.616 / 2, 0.1 - 0.01 / 2 - 0.39, -0.7 + 0.02);
	tuong(0.616, 0.01, 0.02, 165, 42, 42);
	//trai
	instance_h_tt_tuong = Translate(-0.6 + 0.01 / 2, -0.1, -0.7 + 0.02);
	tuong(0.01, 0.4, 0.02, 165, 42, 42);
	//phai
	instance_h_tt_tuong = Translate(0.011, -0.1, -0.7 + 0.02);
	tuong(0.01, 0.4, 0.02, 165, 42, 42);
}
void kinh() {
	setColor(vec4(1, 1, 1, 0.0));
	instance_h_tt_tuong = Translate(-0.59 + 0.606 / 2, 0.09 - 0.38 / 2, -0.7 + 0.02);
	tuong(0.606, 0.38, 0.02, 165, 42, 42);
}
void nengach() {
	//nen nha
	instance_h_tt_tuong = Translate(0, -0.5 + 0.005 / 2, 0);
	tuong(1.3, 0.005, 1.3, 169, 116, 76);
	//nen tuong trai
	instance_h_tt_tuong = Translate(-0.65 + 0.005 / 2, -0.1, 0);
	tuong(0.005, 0.8, 1.3, 238, 232, 170);
	//nen tuong phai
	instance_h_tt_tuong = Translate(0.65 - 0.005 / 2, -0.1, 0);
	tuong(0.005, 0.8, 1.3, 238, 232, 170);//189, 183, 107 mau o liu
	//nen sau
	instance_h_tt_tuong = Translate(0, -0.1, 0.65 - 0.005 / 2);
	tuong(1.3, 0.8, 0.005, 238, 232, 170);
	// nen truoc
	//bo phan tren dinh
	instance_h_tt_tuong = Translate(0, 0.3 - 0.1 / 2, -0.65 + 0.005 / 2);
	tuong(1.3, 0.1, 0.005, 238, 232, 170);
	//bo phan phai sat cua
	instance_h_tt_tuong = Translate(0.65 - 0.042, 0.2 - 0.7 / 2, -0.65 + 0.0025);
	tuong(0.134 - 0.05, 0.7, 0.005, 238, 232, 170);
	//bo phan trai sat cua.
	instance_h_tt_tuong = Translate(0.266 - 0.25 / 2, 0.2 - 0.7 / 2, -0.65 + 0.0025);
	tuong(0.25, 0.7, 0.005, 238, 232, 170);
	//bophan duoi cua so
	instance_h_tt_tuong = Translate(-0.65 + 0.333, -0.5 + 0.1, -0.65 + 0.0025);
	tuong(0.716 - 0.05, 0.2, 0.005, 238, 232, 170);
	//bo phan tren cua so
	instance_h_tt_tuong = Translate(-0.65 + 0.333, 0.2 - 0.1 / 2, -0.65 + 0.0025);
	tuong(0.716 - 0.05, 0.1, 0.005, 238, 232, 170);
	//bo phan trai cua so
	instance_h_tt_tuong = Translate(-0.65 + 0.05 / 2, -0.1, -0.65 + 0.0025);
	tuong(0.05, 0.4, 0.005, 238, 232, 170);
	//4 dinh goc tuong 
	//goc trai
	instance_h_tt_tuong = Translate(-0.65 + 0.005 / 2, 0.4, 0);
	tuong(0.005, 0.2, 1.3, 246, 255, 240);
	//gocphai
	instance_h_tt_tuong = Translate(0.65 - 0.005 / 2, 0.4, 0);
	tuong(0.005, 0.2, 1.3, 246, 255, 240);
	//goc sau 
	instance_h_tt_tuong = Translate(0, 0.4, 0.65 - 0.005 / 2);
	tuong(1.3, 0.2, 0.005, 246, 255, 240);
	//goctruoc
	instance_h_tt_tuong = Translate(0, 0.4, -0.65 + 0.005 / 2);
	tuong(1.3, 0.2, 0.005, 246, 255, 240);
}
void mattruoc() {
	//setColor(vec4(0.96, 0.96, 0.6, 1.0));
	//bo phan tren dinh
	instance_h_tt_tuong = Translate(0, 0.5 - 0.15, -0.7 + 0.025);
	tuong(1.4, 0.3, 0.05, 248, 251, 248);
	//bo phan phai sat cua
	instance_h_tt_tuong = Translate(0.7 - 0.134 / 2, 0.2 - 0.7 / 2, -0.7 + 0.025);
	tuong(0.134, 0.7, 0.05, 248, 251, 248);
	//bo phan trai sat cua.
	instance_h_tt_tuong = Translate(0.266 - 0.25 / 2, 0.2 - 0.7 / 2, -0.7 + 0.025);
	tuong(0.25, 0.7, 0.05, 248, 251, 248);
	//bophan duoi cua so
	instance_h_tt_tuong = Translate(-0.7 + 0.716 / 2, -0.5 + 0.1, -0.7 + 0.025);
	tuong(0.716, 0.2, 0.05, 248, 251, 248);
	//bo phan tren cua so
	instance_h_tt_tuong = Translate(-0.7 + 0.716 / 2, 0.2 - 0.1 / 2, -0.7 + 0.025);
	tuong(0.716, 0.1, 0.05, 248, 251, 248);
	//bo phan trai cua so
	instance_h_tt_tuong = Translate(-0.65 + 0.05 / 2, -0.1, -0.7 + 0.025);
	tuong(0.05, 0.4, 0.05, 248, 251, 248);
}  GLfloat test = 0.0f;
void phongkhach() {
	setColor(vec4(0.96, 0.96, 0.6, 1.0));
	//mat sau
	instance_h_tt_tuong = Translate(0, 0, 0.7 - 0.025);
	tuong(1.4, 1, 0.05, 248, 251, 248);
	//mat trai
	instance_h_tt_tuong = Translate(-0.7 + 0.05 / 2, 0, 0);
	tuong(0.05, 1, 1.4, 248, 251, 248);
	//mat phai
	instance_h_tt_tuong = Translate(0.7 - 0.05 / 2, 0, 0);
	tuong(0.05, 1, 1.4, 248, 251, 248);
	//mat truoc
	mattruoc();
	//mat duoi
	instance_h_tt_tuong = Translate(0, -0.5 - 0.04 / 2, 0);
	tuong(1.4, 0.04, 1.4, 248, 251, 248);
	//mat tren 
	instance_h_tt_tuong = Translate(0, 0.5 + 0.05 / 2 + test, 0);
	tuong(1.4, 0.05, 1.4, 248, 251, 248);
	//khungcua
	h_tt_bpcua = Translate(0.2745, 0.2 - 0.7 / 2, -0.7 + 0.02 + 0.03 / 2);
	khungcua();
	khungcuakinh();
	//kinh();
	//nen phong
	nengach();
	//rem phong
	remcuaso();
	//quat tran
	ttthan = Translate(0, -0.05 + 0.5, 0);
	quattran();

}




// Vẽ tủ tivi
void hinhKhoi(GLfloat x, GLfloat y, GLfloat z) {
	h2_instance = Scale(x, y, z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, h2_model * h2_instance_hop * h2_instance);
	//glUniformMatrix4fv(colors_loc, 1, GL_TRUE, Translate(rc/255,gc/255,bc/255));
	glDrawArrays(GL_TRIANGLES, firstpointlp, numpointslp);
}

void cuaMoTrai9() {
	h2_instance_hop = Translate(-0.19, 0.064, -0.095) * RotateY(beta) * Translate(0.0925, 0.0, 0.0);
	hinhKhoi(0.185, 0.108, 0.01); // mat cua
}

void cuaMoTrai16() {
	h2_instance_hop = Translate(0.2, 0.123, -0.095) * RotateY(beta) * Translate(0.04375, 0.0, 0.0);
	hinhKhoi(0.0875, 0.226, 0.01); // mat cua
}

void cuaMoPhai11() {
	h2_instance_hop = Translate(0.19, 0.064, -0.095) * RotateY(-beta) * Translate(-0.0925, 0.0, 0.0);
	hinhKhoi(0.185, 0.108, 0.01); // mat cua
}

void cuaMoPhai17() {
	h2_instance_hop = Translate(0.39, 0.123, -0.095) * RotateY(-beta) * Translate(-0.04375, 0.0, 0.0);
	hinhKhoi(0.0875, 0.226, 0.01); // mat cua
}

void ngan123456() {
	h2_instance_hop = Translate(-0.2975, 0.418, -0.005);
	hinhKhoi(0.01, 0.344, 0.19); // canh doc
	h2_instance_hop = Translate(-0.2925, 0.477, -0.005);
	hinhKhoi(0.185, 0.01, 0.19); // canh ngang 1
	h2_instance_hop = Translate(-0.2925, 0.359, -0.005);
	hinhKhoi(0.185, 0.01, 0.19); // canh ngang 2
	h2_instance_hop = Translate(-0.2925, 0.241, -0.005);
	hinhKhoi(0.185, 0.01, 0.19); // canh ngang 3

}

void ngan78() {
	h2_instance_hop = Translate(-0.2925, 0.123, -0.005);
	hinhKhoi(0.185, 0.01, 0.19); // canh ngang
}

void tuTrai() {
	setColor(vec4(0.96, 0.64, 0.37, 1.0));
	h2_instance_hop = Translate(-0.395, 0.3, 0.0);
	hinhKhoi(0.01, 0.6, 0.2); // canh trai
	h2_instance_hop = Translate(-0.2, 0.3, 0.0);
	hinhKhoi(0.01, 0.6, 0.2); // canh phai
	h2_instance_hop = Translate(-0.2975, 0.595, 0.0);
	hinhKhoi(0.185, 0.01, 0.2); // canh tren
	h2_instance_hop = Translate(-0.2975, 0.005, 0.0);
	hinhKhoi(0.185, 0.01, 0.2); // canh duoi
	h2_instance_hop = Translate(-0.2975, 0.3, 0.095);
	hinhKhoi(0.185, 0.58, 0.01); // mat sau
	ngan123456();
	ngan78();
}

void ngan12_17() {
	h2_instance_hop = Translate(0.2975, 0.3, -0.005);
	hinhKhoi(0.01, 0.58, 0.19); // canh doc
	h2_instance_hop = Translate(0.2925, 0.359, -0.005);
	hinhKhoi(0.185, 0.01, 0.19); // canh ngang 1
	h2_instance_hop = Translate(0.2925, 0.241, -0.005);
	hinhKhoi(0.185, 0.01, 0.19); // canh ngang 2
}

void tuPhai() {
	setColor(vec4(0.96, 0.64, 0.37, 1.0));
	h2_instance_hop = Translate(0.395, 0.3, 0.0);
	hinhKhoi(0.01, 0.6, 0.2); // canh trai
	h2_instance_hop = Translate(0.2, 0.3, 0.0);
	hinhKhoi(0.01, 0.6, 0.2); // canh phai
	h2_instance_hop = Translate(0.2975, 0.595, 0.0);
	hinhKhoi(0.185, 0.01, 0.2); // canh tren
	h2_instance_hop = Translate(0.2975, 0.005, 0.0);
	hinhKhoi(0.185, 0.01, 0.2); // canh duoi
	h2_instance_hop = Translate(0.2975, 0.3, 0.095);
	hinhKhoi(0.185, 0.58, 0.01); // mat sau
	ngan12_17();
	cuaMoTrai16();
	cuaMoPhai17();
}

void tuGiua() {
	setColor(vec4(0.96, 0.64, 0.37, 1.0));
	h2_instance_hop = Translate(0.0, 0.12, 0.0);
	hinhKhoi(0.395, 0.01, 0.2); // canh tren
	h2_instance_hop = Translate(0.0, 0.005, 0.0);
	hinhKhoi(0.395, 0.01, 0.2); // canh duoi
	h2_instance_hop = Translate(0.0, 0.064, 0.095);
	hinhKhoi(0.395, 0.108, 0.01); // mat sau
	h2_instance_hop = Translate(0.0, 0.064, -0.005);
	hinhKhoi(0.01, 0.108, 0.19); // canh doc
	cuaMoTrai9();
	cuaMoPhai11();
}

void tv() {
	setColor(vec4(0.0 + tv_open, 0.0 + tv_open, 0.0 + tv_open, 1.0));
	h2_instance_hop = Translate(0.0, 0.297, 0.0);
	hinhKhoi(0.37, 0.22, 0.02); // man hinh tv
	h2_instance_hop = Translate(-0.03, 0.16, 0.0) * RotateZ(-20);
	setColor(vec4(0.0, 0.0, 0.0, 1.0));
	hinhKhoi(0.01, 0.08, 0.01); // chan 1
	h2_instance_hop = Translate(0.03, 0.16, 0.0) * RotateZ(20);
	hinhKhoi(0.01, 0.08, 0.01); // chan 2
}

void tu() {
	tuTrai();
	tuPhai();
	tuGiua();
	tv();
}

// ve bo am chen
void amNuoc() {
	setColor(vec4(1.0, 1.0, 1.0, 1.0));
	h2_instance_hop =Translate(-0.18, -0.35, -0.33) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.06, 0.08, 0.06); // than am
	h2_instance_hop = Translate(-0.18, -0.35, -0.33) * Translate(0.23, 0.5 + 0.045, -0.53);
	hinhKhoi(0.03, 0.01, 0.03); // nap am
	h2_instance_hop = Translate(-0.18, -0.35, -0.33) * Translate(0.23, 0.5, -0.53-0.03) * RotateX(-45);
	hinhKhoi(0.01, 0.06, 0.01); // voi am
	h2_instance_hop = Translate(-0.18, -0.35, -0.33) * Translate(0.23, 0.5 + 0.02, -0.53+0.04);
	hinhKhoi(0.01, 0.01, 0.02); // quai tren
	h2_instance_hop = Translate(-0.18, -0.35, -0.33) * Translate(0.23, 0.5 - 0.02, -0.53 + 0.04);
	hinhKhoi(0.01, 0.01, 0.02); // quai duoi
	h2_instance_hop = Translate(-0.18, -0.35, -0.33) * Translate(0.23, 0.5, -0.53 + 0.045);
	hinhKhoi(0.01, 0.04, 0.01); // quai doc

}

void chen() {
	setColor(vec4(1.0, 0.7, 0.8, 1.0));
	h2_instance_hop = Translate(-0.18 - 0.03, -0.35 - 0.015, -0.33 - 0.07) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.03, 0.05, 0.03); 
	h2_instance_hop = Translate(-0.18 + 0.03, -0.35 - 0.015, -0.33 - 0.07) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.03, 0.05, 0.03);
	h2_instance_hop = Translate(-0.18 - 0.03, -0.35 - 0.015, -0.33 - 0.07-0.05) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.03, 0.05, 0.03);
	h2_instance_hop = Translate(-0.18 + 0.03, -0.35 - 0.015, -0.33 - 0.07-0.05) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.03, 0.05, 0.03);
	h2_instance_hop = Translate(-0.18 - 0.03, -0.35 - 0.015, -0.33 - 0.07-0.1) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.03, 0.05, 0.03);
	h2_instance_hop = Translate(-0.18 + 0.03, -0.35 - 0.015, -0.33 - 0.07- 0.1) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.03, 0.05, 0.03);
}

void amChen() {
	amNuoc();
	chen();
}

//ve tranh

void bucTranh() {
	setColor(vec4(0.3, 0.5, 0.7, 1.0));
	h2_instance_hop = Translate(0.63, 0.0, 0.0) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.01, 0.3, 0.6);
}

void khungTranh() {
	setColor(vec4(0.0, 0.0, 0.0, 1.0));
	h2_instance_hop = Translate(0.63, 0.0, -0.31) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.02, 0.34, 0.02);  //khung trai
	h2_instance_hop = Translate(0.63, 0.0, 0.31) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.02, 0.34, 0.02);  //khung trai
	h2_instance_hop = Translate(0.63, 0.16, 0.0) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.02, 0.02, 0.6);  //khung tren
	h2_instance_hop = Translate(0.63, -0.16, 0.0) * Translate(0.23, 0.5, -0.53);
	hinhKhoi(0.02, 0.02, 0.6);  //khung duoi
}

void tranh() {
	bucTranh();
	khungTranh();
}


// Tham

void tham() {
	setColor(vec4(0.2, 0.2, 0.2));
	for (int i = 0; i < 180; i++) {
		h2_instance_hop = Translate(0.48, -0.49, -0.5)  * Translate(0.23, 0.5, -0.53) * RotateY(i);
		hinhKhoi(0.2, 0.01, 0.01);  //than xe
	}
}

GLfloat matz = -1.0, l = -0.5, r = 0.5;
GLfloat bottom = -0.5, top = 0.5;
GLfloat zNear = 0.2f, zFar = 1.0;
mat4 modeleye, mv, p;
void mat() {
	point4 eye(0, 0, matz, 1);
	point4 at(0, 0, 0, 1);
	vec4 up(0, 1, 0, 1);

	mv = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, TRUE, mv * modeleye);

	p = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, TRUE, p);
//	glViewport(0, 0, width, height);
}

void display( void )
{
	
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );                
//	const vec3 viewer_pos(-0.64, 0.4,0 );  /*Trùng với eye của camera*/
	//model = RotateX(gx) * RotateY(gy) * RotateZ(gz);
	h2_model = model * Translate(-0.23, -0.5, 0.53);
	modeleye = RotateX(gx) * RotateY(gy) * RotateZ(gz);
	mat();
	phongkhach();
	amChen();
	tranh();
	tham();
	bobanhoanchinh();
	tu();
	glutSwapBuffers();		


}

void spinqt1(void) {
	spincanhquat += 1.0;
	if (spincanhquat >= 360) spincanhquat -= 360;
	glutPostRedisplay();
}
void spinqt2(void) {
	spincanhquat += 2.0f;
	if (spincanhquat >= 360) spincanhquat -= 360;
	glutPostRedisplay();
}
void spinqt3(void) {
	spincanhquat += 3.0f;
	if (spincanhquat >= 360) spincanhquat -= 360;
	glutPostRedisplay();
}


void keyboard( unsigned char key, int x, int y )
{

	switch (key) {
	case 'h':			
		if (beta < 180 && beta + beta != 180) {
			beta += 10;
		}		
		break;
	case 'H':
		if (beta >= 10) {
			beta -= 10;
		}
		break;
	case '7':
		if (tv_open == 0.0) {
			tv_open += 1.0;
		}
		else {
			tv_open -= 1.0;
		}
		break;

		
	case '1':
		glutIdleFunc(spinqt3);
		break;
	case '2':
		glutIdleFunc(spinqt2);
		break;
	case '3':
		glutIdleFunc(spinqt1);
		break;
	case 'x':
		gx += 5;
		if (gx >= 360) gx -= 360;
		break;
	case 'X':
		gx -= 5;
		if (gx >= 360) gx -= 360;
		break;
	case 'y':
		gy += 5;
		if (gy >= 360) gy -= 360;
		break;
	case 'Y':
		gy -= 5;
		if (gy >= 360) gy -= 360;
		break;
	case 'z':
		gz += 5;
		if (gz >= 360) gz -= 360;
		break;
	case 'Z':
		gz -= 5;
		if (gz >= 360) gz -= 360;
		break;
	case 'm':
		if (cuay > -105 && cuay <= 0) {
			cuay -= 5;
		}
		break;
	case 'M':
		if (cuay < 0 && cuay >= -105) {
			cuay += 5;
		}
		break;
	case 'r':
		if (remy >= 0.008f && remy < 0.63f) remy += 0.02f;
		break;
	case 'R':
		if (remy > 0.01f && remy <= 0.65f) {
			remy -= 0.02f;
		}
		break;
	//View
	case 'l':
		l *= 1.03f;
		r *= 1.03f;
		break;
	case 'L':
		l *= 0.96f;
		r *= 0.96f;
		break;
	case 'p':
		top *= 1.03f; bottom *= 1.03f;
		break;
	case 'P':
		top *= 0.96f; bottom *= 0.96f;
		break;
	case 'o':
		zNear *= 1.03f; zFar *= 1.03f;
		break;
	case 'O':
		zNear *= 0.96f; zFar *= 0.96f;
		break;

	case 'b':
		if (alphaa > -160)
			alphaa -= 5;
		break;
	case 'B':
		if (alphaa <=5)
			alphaa += 5;
		break;
	case 'c':
		if (ax <= 0.14)
			ax += 0.01;
		break;
	case 'C':
		if (ax >= 0.01 )
			ax -= 0.01;
		break;
	}
	glutPostRedisplay();
}


int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 640, 640 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Drawing a Cube" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
    glutKeyboardFunc( keyboard );  
	//glutReshapeFunc(mat);

	//initGL();
	glutMainLoop();
    return 0;
}
