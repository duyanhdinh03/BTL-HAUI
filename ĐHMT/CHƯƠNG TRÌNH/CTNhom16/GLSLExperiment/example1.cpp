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

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints];

point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/


GLuint program;
GLuint Model_loc, Projection_loc, View_loc;
mat4 view, projection, model, instance, ctm_phong;

GLfloat mauBD = 255, mauAnhSang = 1.0, choisang= 0.2;



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

	Model_loc = glGetUniformLocation(program, "Model");
	Projection_loc = glGetUniformLocation(program, "Projection");
	View_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}
//float xeye = 0, yeye = 1, zeye = 1;
//float xup = 0, yup = 1, zup = 0;

void changeColor(GLfloat a, GLfloat b, GLfloat c) {
	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
	point4 light_position(0.0, 2.0, 0.0, 1.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(mauAnhSang, mauAnhSang, mauAnhSang, 1.0);
	color4 light_specular(choisang, choisang, choisang, 1.0);

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(a / 255.0, b / 255.0, c / 255.0, 1.0);
	color4 material_specular(255.0 / 255.0, 255.0 / 255.0, 255 / 255.0, 1.0);
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

mat4 instance_room;
//tuong
void wall_room(GLfloat x, GLfloat y, GLfloat z) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong * instance_room * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
GLfloat xxl, yyl, zzl;
//dung tuong
void wall_build() {
    instance_room = identity();
    //dựng nền căn phòng
    instance_room = Translate(0.0, -2.0,0.0f);
    changeColor(225, 239, 148);
    wall_room(8, 0.05, 11);
    //trần nhà
    instance_room = RotateZ(360) * Translate(0, 2.0, 0.0f);
    changeColor(92, 172, 238);
    wall_room(8, 0.05, 11);
    //tường truoc
    instance_room = Translate(0, 0,5.5f);
    changeColor(225, 296, 155);
    wall_room(8.0, 4, 0.05);
	//tuong sau
	instance_room = Translate(0, 0, -5.5f);
	changeColor(225, 296, 155);
	wall_room(8.0, 4, 0.05);
	//buc giang
	instance_room = Translate(-1.0, -1.83, 5.0f);
	changeColor(225, 239, 148);
	wall_room(6, 0.3, 1.1);

	instance_room = Translate(-3, -1.83, 4.2f);
	changeColor(225, 239, 148);
	wall_room(2, 0.3, 0.5);
	//bang
	instance_room = Translate(0, 0.3, 5.45f);
	changeColor(0, 200, 128);
	wall_room(5, 1.8, 0.05);
    ////tường trái 1
    instance_room = Translate(4.0f, -0.75, 0.0f);
    changeColor(225, 296, 155);
    wall_room(0.05, 2.5, 6.0);
	////tường trái 2
	instance_room = Translate(4.0f, -0.75, 5.0f);
	changeColor(225, 296, 155);
	wall_room(0.05, 2.5, 1.0);
	////tường trái 3
	instance_room = Translate(4.0f, -0.75, -5.0f);
	changeColor(225, 296, 155);
	wall_room(0.05, 2.5, 1.0);
	////tường trái 4
	instance_room = Translate(4.0f, 1.25, 0.0f);
	changeColor(225, 296, 155);
	wall_room(0.05, 1.5, 11);

    ////tường phải1
	instance_room = Translate(-4.0f, 0, 5.0f);
	changeColor(225, 296, 155);
	wall_room(0.05, 1.5, 1.0);
	////tường phải2
	instance_room = Translate(-4.0f, 0, 2.0f);
	changeColor(225, 296, 155);
	wall_room(0.05, 1.5, 2.0);
	////tường phải3
	instance_room = Translate(-4.0f, 0, -1.5f);
	changeColor(225, 296, 155);
	wall_room(0.05, 1.5, 2.0);
	////tường phải4
	instance_room = Translate(-4.0f, 0, -4.75f);
	changeColor(225, 296, 155);
	wall_room(0.05, 1.5, 1.5);
	////tường phải5
	instance_room = Translate(-4.0f, 1.375f, 0.0f);
	changeColor(225, 296, 155);
	wall_room(0.05, 1.25, 11);
	////tường phải6
	instance_room = Translate(-4.0f, -1.375f, 0.0f);
	changeColor(225, 296, 155);
	wall_room(0.05, 1.25, 11);
}

GLfloat mo_cua[] = { 0,0,0,0 };
mat4 instance_cuavao;
void canh_cua(GLfloat x, GLfloat y, GLfloat z) {
    instance = Scale(x, y, z);
    glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong *  instance_room* instance_cuavao * instance);
    glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void cua() {
    instance_cuavao = identity() * Translate(4, -0.75, 3.75) *Translate(0, 0, 0.75) * RotateY(mo_cua[0])* Translate(0,0,-0.375);
    changeColor(252.0, 245.0, 76.0);
    canh_cua(0.05, 2.5, 0.75);
	instance_cuavao = Translate(4, -0.75, 3.75) * Translate(0, 0, -0.75) * RotateY(-mo_cua[0]) * Translate(0, 0, 0.375);
	changeColor(252.0, 245.0, 76.0);
	canh_cua(0.05, 2.5, 0.75);
	instance_cuavao = Translate(4, -0.75, -3.75) * Translate(0, 0, 0.75) * RotateY(mo_cua[1]) * Translate(0, 0, -0.375);
	changeColor(252.0, 245.0, 76.0);
	canh_cua(0.05, 2.5, 0.75);
	instance_cuavao = Translate(4, -0.75, -3.75) * Translate(0, 0, -0.75) * RotateY(-mo_cua[1]) * Translate(0, 0, 0.375);
	changeColor(252.0, 245.0, 76.0);
	canh_cua(0.05, 2.5, 0.75);
}
void bo_cua() {
    instance_room = identity();
    cua();
}

//Cửa sổ
mat4 instance_cuaso;
GLfloat z_cs1 =0, z_cs2 =0, z_cs3 =0;
void canh_cua2(GLfloat x, GLfloat y, GLfloat z) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong * instance_room * instance_cuaso * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void cua2() {
	instance_cuaso = identity() * Translate(-4, 0, 4.125) * Translate(0,0,z_cs1);
	changeColor(252.0, 245.0, 76.0);
	canh_cua2(0.05, 1.5, 0.75);
	instance_cuaso = Translate(-4.05, 0, 3.375) * Translate(0, 0, -z_cs1);
	changeColor(252.0, 245.0, 76.0);
	canh_cua2(0.05, 1.5, 0.75);
	instance_cuaso = Translate(-4, 0, 0.625) * Translate(0, 0, z_cs2);
	changeColor(252.0, 245.0, 76.0);
	canh_cua2(0.05, 1.5, 0.75);
	instance_cuaso = Translate(-4.05, 0, -0.125) * Translate(0, 0, -z_cs2);
	changeColor(252.0, 245.0, 76.0);
	canh_cua2(0.05, 1.5, 0.75);
	instance_cuaso = Translate(-4, 0, -2.875) * Translate(0, 0, z_cs3);
	changeColor(252.0, 245.0, 76.0);
	canh_cua2(0.05, 1.5, 0.75);
	instance_cuaso = Translate(-4.05, 0, -3.625) * Translate(0, 0, -z_cs3);
	changeColor(252.0, 245.0, 76.0);
	canh_cua2(0.05, 1.5, 0.75);
}
void bo_cuaso() {
	instance_room = identity();
	cua2();
}

//=============== VẼ ĐÈN=======================
mat4 instance_den;
bool bat_den = true;
bool view_banGV = true;
void den(GLfloat w, GLfloat h, GLfloat t) {
	instance = Scale(w, h, t);
	glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong * instance_room * instance_den * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void veDen() {
	// tren
	changeColor(255, 215, 0);
	instance_den = Translate(0, 0.03, 0);
	den(1.0, 0.04, 0.15);
	//truoc 
	instance_den = Translate(0, 0, -0.075);
	den(1.0, 0.06, 0.02);
	//sau 
	instance_den = Translate(0, 0, 0.075);
	den(1.0, 0.06, 0.02);
	//trai
	instance_den = Translate(-0.5, 0, 0);
	den(0.04, 0.06, 0.15);
	//phai
	instance_den = Translate(0.5, 0, 0);
	den(0.04, 0.06, 0.15);
	// bong den
	changeColor(mauBD, mauBD, mauBD);
	instance_den = Translate(0, -0.03, 0);
	den(0.9, 0.06, 0.1);
}

void Den_buil() {
	for (int i = 0; i <= 3; i ++) {
		instance_room = identity() * Translate(-2, 1.85, -3 + i +i);
		veDen();
		instance_room = identity() * Translate(2, 1.85, -3 + i +i);
		veDen();
	}
}

// Bộ quạt trần
mat4 instance_quat;
GLfloat daiQ = 0.7;
GLfloat dayQ = 0.025;
GLfloat rongQ = 0.1;
GLfloat quayCanh[] = { 0,0,0,0,0,0,0 };
void quat(GLfloat x, GLfloat y, GLfloat z) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong * instance_room * instance_quat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tru_quat() {
    instance_quat = Translate(0, 0.14, 0);
	changeColor(0, 200, 128);
	quat(0.1, 0.25, 0.1);
}

void canh_quat1() {
	instance_quat = RotateY(quayCanh[1]) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[1]) * RotateY(120) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[1]) * RotateY(240) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);
}

void QuatTran1() {
	tru_quat();
	canh_quat1();
}

void canh_quat2() {
	instance_quat = RotateY(quayCanh[2]) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[2]) * RotateY(120) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[2]) * RotateY(240) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);
}

void QuatTran2() {
	tru_quat();
	canh_quat2();
}

void canh_quat3() {
	instance_quat = RotateY(quayCanh[3]) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[3]) * RotateY(120) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[3]) * RotateY(240) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);
}

void QuatTran3() {
	tru_quat();
	canh_quat3();
}

void canh_quat4() {
	instance_quat = RotateY(quayCanh[4]) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[4]) * RotateY(120) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[4]) * RotateY(240) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);
}

void QuatTran4() {
	tru_quat();
	canh_quat4();
}

void canh_quat5() {
	instance_quat = RotateY(quayCanh[5]) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[5]) * RotateY(120) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[5]) * RotateY(240) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);
}

void QuatTran5() {
	tru_quat();
	canh_quat5();
}

void canh_quat6() {
	instance_quat = RotateY(quayCanh[6]) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[6]) * RotateY(120) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);

	instance_quat = RotateY(quayCanh[6]) * RotateY(240) * Translate(0.5 * daiQ, 0, 0);
	changeColor(0, 200, 128);
	quat(daiQ, dayQ, rongQ);
}

void QuatTran6() {
	tru_quat();
	canh_quat6();
}
void bo_quat() {
    instance_room = Translate(1, 1.7, 2);
	QuatTran1();
	instance_room = Translate(-1, 1.7, 2);
	QuatTran2();
	instance_room = Translate(1, 1.7, 0);
	QuatTran3();
	instance_room = Translate(-1, 1.7, 0);
	QuatTran4();
	instance_room = Translate(1, 1.7, -2);
	QuatTran5();
	instance_room = Translate(-1, 1.7, -2);
	QuatTran6();
}

// Bo dieu hoa
mat4 instance_dieuhoa;
void dh(GLfloat x, GLfloat y, GLfloat z) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong * instance_room * instance_dieuhoa * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void than_dh() {
	// mat trc
	instance_dieuhoa = Translate(0, 0, 0);
	dh(1, 0.4, 0.05);
	//mat sau
	instance_dieuhoa = Translate(0, -0.1, 0.3);
	dh(1, 0.6, 0.05);
	//mat tren
	instance_dieuhoa = Translate(0, 0.175, 0.15);
	dh(1, 0.05, 0.25);
	//mat duoi
	instance_dieuhoa = Translate(0, -0.375, 0.15);
	dh(1, 0.05, 0.25);
	//mat trai
	instance_dieuhoa = Translate(0.525, -0.1, 0.15);
	dh(0.05, 0.6, 0.35);
	// mat phai
	instance_dieuhoa = Translate(-0.525, -0.1, 0.15);
	dh(0.05, 0.6, 0.35);
}

GLfloat quaydh[] = { 0,0,0,0 };
void canh_dh1() {
	instance_dieuhoa = Translate(0, -0.2, 0) * RotateX(quaydh[1]) * Translate(0, -0.1, 0); 
	dh(1, 0.2, 0.05);
}

void dieuhoa1(){
	than_dh();
	canh_dh1();
}

void canh_dh2() {
	instance_dieuhoa = Translate(0, -0.2, 0) * RotateX(quaydh[2]) * Translate(0, -0.1, 0);
	dh(1, 0.2, 0.05);
}

void dieuhoa2() {
	than_dh();
	canh_dh2();
}

void bo_dh() {
	instance_room = Translate(-3.6, 1.5, 2) * RotateY(-90);
	dieuhoa1();
	instance_room = Translate(-3.6, 1.5, -2) * RotateY(-90);
	dieuhoa2();
}

//Bàn học
mat4 instance_ban;
void ban(GLfloat x, GLfloat y, GLfloat z) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong * instance_room * instance_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void matBan() {
	instance_ban = Translate(0, 0.3, 0);
	changeColor(252.0, 245.0, 76.0);
	ban(0.8, 0.02, 0.4);
}

void chanBan() {
	//chan 1
	instance_ban = Translate(-0.375, -0.01, -0.2);
	changeColor(225, 296, 155);
	ban(0.02, 0.6, 0.02);

	//chan 2
	instance_ban = Translate(-0.375, -0.01, 0.2);
	changeColor(225, 296, 155);
	ban(0.02, 0.6, 0.02);

	//chan 3
	instance_ban = Translate(0.375, -0.01, -0.2);
	changeColor(225, 296, 155);
	ban(0.02, 0.6, 0.02);

	//chan 4
	instance_ban = Translate(0.375, -0.01, 0.2);
	changeColor(225, 296, 155);
	ban(0.02, 0.6, 0.02);

	//thang doc 1
	instance_ban = Translate(-0.375, -0.24, 0);
	changeColor(225, 296, 155);
	ban(0.02, 0.02, 0.4);

	//thanh doc 2
	instance_ban = Translate(0.375, -0.24, 0);
	changeColor(225, 296, 155);
	ban(0.02, 0.02, 0.4);

	//thanh ngang
	instance_ban = Translate(0, -0.24, 0);
	changeColor(225, 296, 155);
	ban(0.78, 0.02, 0.02);
}

void matGhe() {
	instance_ban = Translate(0, 0, 0.5);
	changeColor(252.0, 245.0, 76.0);
	ban(0.8, 0.02, 0.25);
}

void chanGhe() {
	
	//chan 1
	instance_ban = Translate(-0.375, -0.15, 0.4);
	changeColor(225, 296, 155);
	ban(0.02, 0.3, 0.02);

	//chan 2
	instance_ban = Translate(-0.375, -0.15, 0.6);
	changeColor(225, 296, 155);
	ban(0.02, 0.3, 0.02);

	//chan 3
	instance_ban = Translate(0.375, -0.15, 0.4);
	changeColor(225, 296, 155);
	ban(0.02, 0.3, 0.02);

	//chan 4
	instance_ban = Translate(0.375, -0.15, 0.6);
	changeColor(225, 296, 155);
	ban(0.02, 0.3, 0.02);

	//thang doc 1
	instance_ban = Translate(-0.375, -0.24, 0.4);
	changeColor(225, 296, 155);
	ban(0.02, 0.02, 0.4);

	//thanh doc 2
	instance_ban = Translate(0.375, -0.24, 0.4);
	changeColor(225, 296, 155);
	ban(0.02, 0.02, 0.4);

	//thanh ngang
	instance_ban = Translate(0, -0.24, 0.5);
	changeColor(225, 296, 155);
	ban(0.78, 0.02, 0.02);
}

void caiBan() {
	matBan();
	chanBan();
	chanGhe();
	matGhe();
}

void dayban() {
	instance_room = Translate(2.5, -1.69, 2) * RotateY(180);
	caiBan();
	instance_room = Translate(1, -1.69, 2) * RotateY(180);
	caiBan();
	instance_room = Translate(-1, -1.69, 2) * RotateY(180);
	caiBan();
	instance_room = Translate(-2.5, -1.69, 2) * RotateY(180);
	caiBan();

	instance_room = Translate(2.5, -1.69, 1) * RotateY(180);
	caiBan();
	instance_room = Translate(1, -1.69, 1) * RotateY(180);
	caiBan();
	instance_room = Translate(-1, -1.69, 1) * RotateY(180);
	caiBan();
	instance_room = Translate(-2.5, -1.69, 1) * RotateY(180);
	caiBan();

	instance_room = Translate(2.5, -1.69, 0) * RotateY(180);
	caiBan();
	instance_room = Translate(1, -1.69, 0) * RotateY(180);
	caiBan();
	instance_room = Translate(-1, -1.69, 0) * RotateY(180);
	caiBan();
	instance_room = Translate(-2.5, -1.69, 0) * RotateY(180);
	caiBan();

	instance_room = Translate(2.5, -1.69, -1) * RotateY(180);
	caiBan();
	instance_room = Translate(1, -1.69, -1) * RotateY(180);
	caiBan();
	instance_room = Translate(-1, -1.69, -1) * RotateY(180);
	caiBan();
	instance_room = Translate(-2.5, -1.69, -1) * RotateY(180);
	caiBan();

	instance_room = Translate(2.5, -1.69, -2) * RotateY(180);
	caiBan();
	instance_room = Translate(1, -1.69, -2) * RotateY(180);
	caiBan();
	instance_room = Translate(-1, -1.69, -2) * RotateY(180);
	caiBan();
	instance_room = Translate(-2.5, -1.69, -2) * RotateY(180);
	caiBan();
}

//ban giao vien
mat4 instance_bangv;
void bangv(GLfloat x, GLfloat y, GLfloat z) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong * instance_room * instance_bangv * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void matBanGV() {
	instance_bangv = Translate(0, 0, 0);
	changeColor(252.0, 245.0, 76.0);
	bangv(0.8, 0.02, 0.4);
}

void chanBanGV() {
	//chan 1
	instance_bangv = Translate(0.39, -0.31, 0.19);
	changeColor(225, 296, 155);
	bangv(0.02, 0.6, 0.02);

	//chan 2
	instance_bangv = Translate(0.39, -0.31, -0.19);
	changeColor(225, 296, 155);
	bangv(0.02, 0.6, 0.02);

	//chan 3
	instance_bangv = Translate(-0.39, -0.31, 0.19);
	changeColor(225, 296, 155);
	bangv(0.02, 0.6, 0.02);

	//chan 4
	instance_bangv = Translate(-0.39, -0.31, -0.19);
	changeColor(225, 296, 155);
	bangv(0.02, 0.6, 0.02);
}

GLfloat goctu = 0;
void hoctu() {
	instance_bangv = Translate(0, -0.26, 0.19);
	changeColor(252.0, 245.0, 76.0);
	bangv(0.76, 0.5, 0.02);

	instance_bangv = Translate(0.39, -0.26, 0);
	changeColor(252.0, 245.0, 76.0);
	bangv(0.02, 0.5, 0.36);

	instance_bangv = Translate(-0.39, -0.26, 0);
	changeColor(252.0, 245.0, 76.0);
	bangv(0.02, 0.5, 0.36);

	instance_bangv = Translate(-0.1, -0.26, 0);
	changeColor(252.0, 245.0, 76.0);
	bangv(0.02, 0.5, 0.36);

	instance_bangv = Translate(-0.245, -0.5, 0);
	changeColor(252.0, 245.0, 76.0);
	bangv(0.27, 0.02, 0.36);

	instance_bangv = Translate(-0.39, -0.26, -0.19) * RotateY(goctu) * Translate(0.145, 0, 0);
	changeColor(252.0, 245.0, 76.0);
	bangv(0.29, 0.5, 0.02);
}

void caibanGV()
{
	matBanGV();
	chanBanGV();
	hoctu();
}

void botriBanGV() {
	instance_room = Translate(-3, -1, 4.3) * RotateY(180);
	caibanGV();
}

//ghegv
mat4 instance_ghegv;
void ghegv(GLfloat x, GLfloat y, GLfloat z) {
	instance = Scale(x, y, z);
	glUniformMatrix4fv(Model_loc, 1, GL_TRUE, ctm_phong * instance_room * instance_ghegv * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void matgheGV() {
	instance_ghegv = Translate(0, 0, 0);
	changeColor(252.0, 245.0, 76.0);
	ghegv(0.3, 0.02, 0.3);
}

void changheGV() {
	//chan 1
	instance_ghegv = Translate(0.14, -0.16, 0.14);
	changeColor(225, 296, 155);
	ghegv(0.02, 0.3, 0.02);

	//chan 2
	instance_ghegv = Translate(0.14, -0.16, -0.14);
	changeColor(225, 296, 155);
	ghegv(0.02, 0.3, 0.02);

	//chan 3
	instance_ghegv = Translate(-0.14, -0.16, 0.14);
	changeColor(225, 296, 155);
	ghegv(0.02, 0.3, 0.02);

	//chan 4
	instance_ghegv = Translate(-0.14, -0.16, -0.14);
	changeColor(225, 296, 155);
	ghegv(0.02, 0.3, 0.02);
}

void lunggheGV() {
	instance_ghegv = Translate(0, 0.21, 0.14);
	changeColor(252.0, 245.0, 76.0);
	ghegv(0.3, 0.4, 0.02);
}

void caighegv() {
	matgheGV();
	changheGV();
	lunggheGV();
}

GLfloat xoayghe = 0;
GLfloat zg;
void botrighegv() {
	instance_room = Translate(-3.2, -1.3, 4.5) * Translate( 0, 0, zg) * RotateY(xoayghe);
	caighegv();
}

// Can phong
void room() {
	bo_cua();
	bo_cuaso();
	wall_build();
	Den_buil();
	bo_quat();
	bo_dh();
	dayban();
	botriBanGV();
	botrighegv();
}

GLfloat l = -2.5, r = 2.5;   // giới hạn của khung nhìn
GLfloat bottom = -2.5, top = 2.5;  //giới hạn khung nhìn
GLfloat zNear = 1, zFar = 17; // giới hạn khoảng cách hiển thị được
GLfloat z_eye = -10.0, y_eye = 1; //điểm view x,y
GLdouble xx_p, yy_ph, zz_p;
GLfloat x_phong = 0, y_phong = 0, z_phong = -4.4;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const vec3 viewer_pos(0, y_eye, z_eye);  /*Trùng với eye của camera*/

	ctm_phong = Translate(x_phong, y_phong, z_phong) * RotateY(yy_ph);
	room();
	vec4 eye(0, y_eye, z_eye, 1);          //điểm nhìn
	vec4 at(0, 0, 0, 1);
	vec4 up(0, 1, 0, 1);
	view = LookAt(eye, at, up);
	glUniformMatrix4fv(View_loc, 1, GL_TRUE, view);

	projection = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(Projection_loc, 1, GL_TRUE, projection);
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
	case 'x':
		x_phong += 0.1;
		glutPostRedisplay();
		break;

	case 'y':
		y_phong += 0.1;
		glutPostRedisplay();
		break;

	case 'z':
		z_phong += 0.1;
		glutPostRedisplay();
		break;

	case 'X':
		x_phong -= 0.1;
		glutPostRedisplay();
		break;

	case 'Y':
		y_phong -= 0.1;
		glutPostRedisplay();
		break;

	case 'Z':
		z_phong -= 0.1;
		glutPostRedisplay();
		break;

		//quay phòng
	case 'q':
		yy_ph += 10;
		glutPostRedisplay();
		break;

	case 'Q':

		yy_ph -= 10;
		glutPostRedisplay();
		break;
		break;
		
		//mở đóng cửa ở bục giảng
	case 'c':
		mo_cua[0] -= 5;
		if (mo_cua[0] <= -170) { mo_cua[0] -= (mo_cua[0] + 170); }
		glutPostRedisplay();
		break;

	case 'C':
		mo_cua[0] += 5;
		if (mo_cua[0] >= 0) { mo_cua[0] =  0; }
		glutPostRedisplay();
		break;

		//mở đóng cửa ở cuối phòng
	case 'v':
		mo_cua[1] -= 5;
		if (mo_cua[1] <= -170) { mo_cua[1] -= (mo_cua[1] + 170); }
		glutPostRedisplay();
		break;

	case 'V':
		mo_cua[1] += 5;
		if (mo_cua[1] >= 0) { mo_cua[1] = 0; }
		glutPostRedisplay();
		break;

		//mở đóng cửa sổ bục giảng(cửa sổ 1)
	case 'b':
		z_cs1 -= 0.05;
		if (z_cs1 <= -0.5) z_cs1 = -0.5;
		glutPostRedisplay();
		break;

	case 'B':
		z_cs1 += 0.05;
		if (z_cs1 >= 0) z_cs1 = 0;
		glutPostRedisplay();
		break;
		//cửa sổ 2
	case 'n':
		z_cs2 -= 0.05;
		if (z_cs2 <= -0.5) z_cs2 = -0.5;
		glutPostRedisplay();
		break;

	case 'N':
		z_cs2 += 0.05;
		if (z_cs2 >= 0) z_cs2 = 0;
		glutPostRedisplay();
		break;

		//cửa sổ 3
	case 'm':
		z_cs3 -= 0.05;
		if (z_cs3 <= -0.5) z_cs3 = -0.5;
		glutPostRedisplay();
		break;

	case 'M':
		z_cs3 += 0.05;
		if (z_cs3 >= 0) z_cs3 = 0;
		glutPostRedisplay();
		break;

		// bat den
	case 't':
		if (bat_den) {
			mauBD = 100;
			mauAnhSang = 0.2;
			bat_den = false;
		}
		else {
			mauBD = 255;
			mauAnhSang = 1;
			bat_den = true;
		}
		glutPostRedisplay();
		break;
		
		// bat quat
    case '1':
		quayCanh[1] += 10;
		if (quayCanh[1] < 360) {
			quayCanh[1] += 10;
		}
		glutPostRedisplay();
		break;
	case '2':
		quayCanh[2] += 10;
		if (quayCanh[2] < 360) {
			quayCanh[2] += 10;
		}
		glutPostRedisplay();
		break;
	case '3':
		quayCanh[3] += 10;
		if (quayCanh[3] < 360) {
			quayCanh[3] += 10;
		}
		glutPostRedisplay();
		break;
	case '4':
		quayCanh[4] += 10;
		if (quayCanh[4] < 360) {
			quayCanh[4] += 10;
		}
		glutPostRedisplay();
		break;
	case '5':
		quayCanh[5] += 10;
		if (quayCanh[5] < 360) {
			quayCanh[5] += 10;
		}
		glutPostRedisplay();
		break;
	case '6':
		quayCanh[6] += 10;
		if (quayCanh[6] < 360) {
			quayCanh[6] += 10;
		}
		glutPostRedisplay();
		break;

		// bat,tat dh
	case 'o':
		quaydh[1] += 5;
		if (quaydh[1] >= 80) {
			quaydh[1] = 80;
		}
		glutPostRedisplay();
		break;

	case 'O':
		quaydh[1] -= 5;
		if (quaydh[1] <= 0) {
			quaydh[1] = 0;
		}
		glutPostRedisplay();
		break;

	case 'p':
		quaydh[2] += 5;
		if (quaydh[2] >= 80) {
			quaydh[2] = 80;
		}
		glutPostRedisplay();
		break;

	case 'P':
		quaydh[2] -= 5;
		if (quaydh[2] <= 0) {
			quaydh[2] = 0;
		}
		glutPostRedisplay();
		break;

	case 'l':
		goctu += 5;
		if (goctu > 90) goctu = 90;
		glutPostRedisplay();
		break;

	case 'L':
		goctu -= 5;
		if (goctu <= 0) goctu = 0;
		glutPostRedisplay();
		break;

	case 'k':
		xoayghe += 1;
		if (xoayghe < 360) {
			xoayghe += 1;
		}
		glutPostRedisplay();
		break;

	case 'j':
		zg += 0.1;
		if (zg > 0.3) zg = 0.3;
		glutPostRedisplay();
		break;

	case 'J':
		zg -= 0.1;
		if (zg < 0) zg = 0;
		glutPostRedisplay();
		break;
	}


}


int main(int argc, char** argv)
{
	// main function: program starts here

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Phong Hoc A9");


	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}
