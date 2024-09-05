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

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints]; /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program,view_loc,projection_loc;

GLfloat la = 1.0, lb = 1.0, lc = 1.0;
/* Khởi tạo các tham số chiếu sáng - tô bóng*/
point4 light_position(0.0, 20, 20, 0.0);
color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(la, lb, lc, 1.0); //màu nguồn
color4 light_specular(1.0, 1.0, 1.0, 1.0); // màu as px

color4 material_ambient(0.5, 0.5, 0.5, 1.0);
color4 material_diffuse(0.0, 1.0, 0.0, 1.0);//màu vật
color4 material_specular(1.0, 1.0, 1.0, 1.0); // màu phản xạ
float material_shininess = 1000.0; // độ bóng

vec4 ambient_product, diffuse_product, specular_product;

// Đặt biến toàn cục để lưu trữ giá trị màu
GLfloat denXeColorR = 207 / 255.0;
GLfloat denXeColorG = 208 / 255.0;
GLfloat denXeColorB = 200 / 255.0;

void setDenXeColor(GLfloat r, GLfloat g, GLfloat b)
{
	denXeColorR = r;
	denXeColorG = g;
	denXeColorB = b;
}

void setMauSP(GLint r, GLint g, GLint b)
{
	material_diffuse = vec4(1.0 * r / 255, 1.0 * g / 255, 1.0 * b / 255, 1.0);
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
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
void generateGeometry( void )
{
	initCube();
	makeColorCube();
}

mat4 instance;
//KBAO MODEL
mat4 model;
mat4 modelPhong;
mat4 modelBan;
mat4 modelMay;
mat4 modelQuat;
mat4 modelRobot;

GLuint model_view_loc;

//KBAO OTO
GLfloat a = 0, b = 0, c = 0, d = 0, xt = 0, yt = 0, angle_gatMua=0;
//KBAO BAN LAM VIEC
GLfloat k = 0;
//KBAO QUAT TRAN
GLfloat q = 0, speed =0;
//KBAO ROBOT
GLfloat h = 0, j = 0, o = 180, p = 0;

bool gatMuaEnabled = false;

//ve o to
void dauXe()
{
	setMauSP(255, 0, 0);
	instance = Scale(0.3, 0.2, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void duoiXe()
{
	setMauSP(255, 0, 0);
	instance = Translate(0.8, 0, 0) * Scale(0.3, 0.2, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void kinhTruoc()
{
	setMauSP(0, 171, 255);
	instance = Translate(0.2, 0.18, 0) * RotateZ(-30) * Scale(0.02, 0.2, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
 
void gatMua()
{
	setMauSP(0, 0, 0);
	instance = Translate(0.18, 0.18, 0) * RotateZ(-30) * RotateX(angle_gatMua) * Scale(0.02, 0.15, 0.02);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void kinhSau()
{
	setMauSP(0, 171, 255);
	instance = Translate(0.66, 0.15, 0) * Scale(0.02, 0.1, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
 
void drawWheel(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat radius) {
	char count = 16;
	GLfloat angle = 90.0f / count;
	GLfloat edge = 2 * radius / sqrt(2);

	setMauSP(0, 0, 0);
	for (char i = 0; i < count; ++i) {
		instance = RotateY(90) * Translate(tx, ty, tz) * RotateX(i * angle) * Scale(0.05, edge, edge);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

void gamXe()
{
	setMauSP(0, 0, 0);
	instance = Translate(0.4, -0.09, 0) * Scale(0.5, 0.02, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
	
	
	drawWheel(-0.15, -0.1, 0.8, 0.1);//banh sau 
	drawWheel(0.15, -0.1, 0.8, 0.1);//banh sau
}

void matGhe(GLfloat x, GLfloat y, GLfloat z)
{
	setMauSP(240, 229, 174);
	instance = Translate(x, y, z) * Scale(0.2, 0.05, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void tuaGhe(GLfloat x, GLfloat y, GLfloat z)
{
	setMauSP(240, 229, 174);
	instance = Translate(x, y, z) * RotateZ(-20) * Scale(0.02, 0.3, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void ghe()
{
	matGhe(0.4, -0.07, 0);
	tuaGhe(0.54,0.1,0);
}

void denXe(GLfloat x, GLfloat y, GLfloat z)
{
	setMauSP(denXeColorR * 255, denXeColorG * 255, denXeColorB * 255);
	instance = Translate(x, y, z) * Scale(0.02, 0.08, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints); /*Vẽ các tam giác*/
}

void khungXe()
{
	dauXe();
	duoiXe();
	kinhTruoc();
	kinhSau();
	gamXe();
	ghe();
	
}

void cuaXe1()
{
	setMauSP(212, 65, 65);
	instance = Translate(0.4, 0, 0.14) * Scale(0.5, 0.2, 0.02);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void cuaXe2()
{
	setMauSP(212, 65, 65);
	instance = Translate(0.4, 0, -0.14) * Scale(0.5, 0.2, 0.02);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void cuaXe()
{
	cuaXe1();
	cuaXe2();
}

void xeMuiTran(GLfloat x, GLfloat y, GLfloat z)
{
	model = Translate(x, y, z) * Translate(xt, 0, yt) * RotateY(a) * RotateX(b);
	khungXe();
	gatMua();
	denXe(-0.15, 0, 0);
	mat4 than = model;
	model = than * Translate(0.15, 0, 0) * RotateZ(c) * Translate(-0.15, 0, 0);
	cuaXe();
	model = than * Translate(0, 0, 0.15) * RotateY(d) * Translate(0, 0, -0.15);
	drawWheel(-0.15, -0.1, 0, 0.1);
	model = than * Translate(0, 0, -0.15) * RotateY(d) * Translate(0, 0, 0.15);
	drawWheel(0.15, -0.1, 0, 0.1);


}

//ve phong
void tuongTrai()
{
	setMauSP(245, 249, 195);
	instance = Translate(-2.5, 0, 0) * Scale(0.05, 5, 5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void tuongPhai()
{
	setMauSP(245, 249, 195);
	instance = Translate(2.5, 0, 0) * Scale(0.05, 5, 5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void tuongSau()
{
	setMauSP(245, 249, 195);
	instance = Translate(0, 0, -2.5) * Scale(5, 5, 0.05);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void tuongTruoc()
{
	setMauSP(245, 249, 195);
	instance = Translate(0, 0, 2.5) * Scale(5, 5, 0.05);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void sanNha()
{
	setMauSP(245, 249, 195);
	instance = Translate(0, -2.5, 0) * Scale(5, 0.05, 5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void nocNha()
{
	setMauSP(245, 249, 195);
	instance = Translate(0, 2.5, 0) * Scale(5, 0.05, 5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanhDoc()
{
	setMauSP(169, 167, 167);
	instance = Translate(0, 0, -2) * Scale(0.1, 5, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanhDoc1()
{
	setMauSP(169, 167, 167);
	instance = Translate(-2.49, 0, -2) * Scale(0.1, 5, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanhDoc2()
{
	setMauSP(169, 167, 167);
	instance = Translate(2.49, 0, -2) * Scale(0.1, 5, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanhNgang()
{
	setMauSP(169, 167, 167);
	instance = Translate(0, -0.4, -2) * Scale(5, 0.1, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanhNgang1()
{
	setMauSP(169, 167, 167);
	instance = Translate(0, 2.48, -2) * Scale(5, 0.1, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanhNgang2()
{
	setMauSP(169, 167, 167);
	instance = Translate(0, -2.48, -2) * Scale(5, 0.1, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void denDien()
{
	setMauSP(255, 255, 255);
	instance = Translate(-1.2, 1.5, -2.0) * Scale(1, 0.2, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelPhong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void phong()
{
	tuongTrai();
	tuongPhai();
	tuongSau();
	tuongTruoc();
	sanNha();
	nocNha();
	thanhDoc();
	thanhDoc1();
	thanhDoc2();
	thanhNgang();
	thanhNgang1();
	thanhNgang2();
	denDien();
}

//code ve ban thanh toan
void matBan()
{
	setMauSP(255, 255, 255);
	instance = Scale(1.2, 0.02, 0.6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void chan1()
{
	setMauSP(0, 0, 0);
	instance = Translate(-0.57, -0.31, -0.27) * Scale(0.06, 0.6, 0.06);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void chan2()
{
	setMauSP(0, 0, 0);
	instance = Translate(-0.57, -0.31, 0.27) * Scale(0.06, 0.6, 0.06);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void chan3()
{
	setMauSP(0, 0, 0);
	instance = Translate(0.57, -0.31, 0.27) * Scale(0.06, 0.6, 0.06);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void chan4()
{
	setMauSP(0, 0, 0);
	instance = Translate(0.57, -0.31, -0.27) * Scale(0.06, 0.6, 0.06);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tamChan1()
{
	setMauSP(105, 83, 83);
	instance = Translate(0.39, -0.21, 0.29) * Scale(0.3, 0.4, 0.02);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tamChan2()
{
	setMauSP(105, 83, 83);
	instance = Translate(0.39, -0.21, -0.29) * Scale(0.3, 0.4, 0.02);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tamChan3()
{
	setMauSP(105, 83, 83);
	instance = Translate(0.53, -0.21, 0) * Scale(0.02, 0.4, 0.6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tamChan4()
{
	setMauSP(105, 83, 83);
	instance = Translate(0.23, -0.21, 0) * Scale(0.02, 0.4, 0.6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tamChan5()
{
	setMauSP(105, 83, 83);
	instance = Translate(0.39, -0.41, 0) * Scale(0.32, 0.02, 0.6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canhTu()
{
	setMauSP(105, 83, 83);
	instance = Translate(0.39, -0.21, -0.29) * Scale(0.3, 0.4, 0.02);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void nganKeo()
{
	setMauSP(105, 83, 83);
	instance = Scale(0.6, 0.02, 0.6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelBan * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void khungBan()
{
	matBan();
	chan1();
	chan2();
	chan3();
	chan4();
	tamChan1();
	tamChan3();
	tamChan4();
	tamChan5();
}

void ban()
{
	modelBan = Translate(1,-1.5,0.5);
	khungBan();
	canhTu();
	modelBan = modelBan * Translate(-0.24, -0.1, 0) * Translate(0, 0, k);
	nganKeo();
}

//ve man hinh may tinh
void manHinh()
{
	setMauSP(81, 81, 81);
	instance = Translate(0.9,-1.2,0.5) * Scale(0.5, 0.5, 0.01);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelMay * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void cayPC()
{
	setMauSP(81, 81, 81);
	instance = Translate(1.4, -1.2, 0.5) * Scale(0.3, 0.5, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelMay * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void banPhim()
{
	setMauSP(0, 0, 0);
	instance = Translate(0.9, -1.45, 0.65) * Scale(0.2, 0.01, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelMay * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void PC()
{
	manHinh();
	cayPC();
	banPhim();
}

//ve quat tran
void canh1()
{
	setMauSP(149, 242, 236);
	instance = Scale(1, 0.02, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelQuat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canh2()
{
	setMauSP(149, 242, 236);
	instance = RotateY(120) * Scale(1, 0.02, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelQuat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canh3()
{
	setMauSP(149, 242, 236);
	instance = RotateY(240) * Scale(1, 0.02, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelQuat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void truQuat()
{
	setMauSP(105, 83, 83);
	instance = Translate(0,0.16,0) * Scale(0.1, 0.3, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelQuat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void quat()
{
	modelQuat = Translate(0, 2, 0);
	truQuat();
	modelQuat = modelQuat * RotateY(q);
	canh1();
	canh2();
	canh3();
}

//ve optimus
void thanRobot()
{
	setMauSP(255, 0, 0);
	instance = Scale(0.8, 0.9, 0.6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelRobot * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void dauRobot()
{
	setMauSP(0, 0, 255);
	instance = Translate(0.0, 0.6, 0) * Scale(0.3, 0.3, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelRobot * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void chanRB1()
{
	setMauSP(255, 255, 0);
	instance = Translate(0.2, -0.85, 0) * Scale(0.3, 0.8, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelRobot * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void chanRB2()
{
	setMauSP(255, 255, 0);
	instance = Translate(-0.2, -0.85, 0) * Scale(0.3, 0.8, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelRobot * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void ngucRobot()
{
	setMauSP(255, 255, 0);
	instance = Translate(0, 0.2, 0.3) * Scale(0.6, 0.1, 0.05);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelRobot * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void khungRobot()
{
	thanRobot();
	ngucRobot();
	chanRB1();
	chanRB2();
}

void tay1()
{
	setMauSP(255, 153, 204);
	instance = Translate(0.51, -0.1, 0) * Scale(0.2, 0.9, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelRobot * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void tay2()
{
	setMauSP(255, 153, 204);
	instance = Translate(-0.51, -0.1, 0) * Scale(0.2, 0.9, 0.3);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, modelRobot * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void tayRobot()
{
	tay1();
	tay2();
}

void Robot(GLfloat x, GLfloat y, GLfloat z)
{
	modelRobot = Translate(x,y,z) * RotateY(h) * RotateX(j);
	khungRobot();
	mat4 than = modelRobot;
	modelRobot = than *  Translate(0.15, 0.2, 0) * RotateX(o) * Translate(-0.15, 0.5, 0);
	tayRobot();
	modelRobot = than * RotateY(p);
	dauRobot();

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

void shaderSetup( void )
{
	// Nạp các shader và sử dụng chương trình shader
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );   // hàm InitShader khai báo trong Angel.h
    glUseProgram( program );

    // Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
    GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc_vPosition );
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	model_view_loc = glGetUniformLocation(program, "Model");
	view_loc = glGetUniformLocation(program, "View");
	projection_loc = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

GLfloat xx=0, yy=0, zz=2.7;
GLfloat xa = 0, ya = 0;
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Sử dụng hàm LookAt để đặt vị trí và hướng nhìn của camera
	mat4 view = LookAt(
		vec4(xx, yy, zz, 1),
		vec4(0, 0, 1.5 - 0.1, 1),
		vec4(0, 1, 0, 1)
	);

	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	// Sử dụng hàm Frustum để tạo ma trận chiếu phối cảnh 3D xuống 2D
	mat4 projection = Frustum(-1, 1, -1, 1, 1.5, 30);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	//ve phong 
	phong();

	if (gatMuaEnabled)
	{
		angle_gatMua += 0.05;
		if (angle_gatMua > 30.0)
		{
			angle_gatMua = -30.0;
		}

	}
	// Vẽ xe mui trần
	xeMuiTran(-1.5, -0.15, -2);

	//ban
	ban();

	//pc
	PC();

	//quat tran
	q = q + speed;
	quat();

	//robot
	Robot(1.2,0.9,-2);
	
	// Hiển thị các thay đổi
	glutPostRedisplay();
	glutSwapBuffers();
}

bool CONTROL = true;
void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

	switch (key)
	{
		case 033:			// 033 is Escape key octal value
			exit(1);		// quit program
			break;
		case 'u':
			CONTROL = true;
			break;
		case 'U':
			CONTROL = false;
			break;
	}

	if (CONTROL == true)
	{
		switch(key)
		{
		//NUT CHO OTO 
		case'a'://XOAY XE
			a = a + 5;
			glutPostRedisplay();
			break;
		case'A':
			a = a - 5;
			glutPostRedisplay();
			break;
		case'b':
			b = b + 5;
			glutPostRedisplay();
			break;
		case'B':
			b = b - 5;
			glutPostRedisplay();
			break;
		case 'c'://MO CUA XE
			c = c + 5;
			if (c > 50)
				c = 50;
			glutPostRedisplay();
			break;
		case 'C':
			c = 0;
			glutPostRedisplay();
			break;
		case 'd'://XOAY BANH LAI
			d = d + 5;
			if (d > 50)
				d = 50;
			glutPostRedisplay();
			break;
		case 'D':
			d = d - 5;
			if (d < -50)
				d = -50;
			glutPostRedisplay();
			break;
		case't'://di chuyen
			xt = xt + 0.1;
			glutPostRedisplay();
			break;
		case'T':
			xt = xt - 0.1;
			glutPostRedisplay();
			break;
		case'r':
			yt = yt + 0.1;
			glutPostRedisplay();
			break;
		case'R':
			yt = yt - 0.1;
			glutPostRedisplay();
			break;
		case'x'://GOC NHIN CAMERA
			xx = xx + 0.1;
			glutPostRedisplay();
			break;
		case'X':
			xx = xx - 0.1;
			glutPostRedisplay();
			break;
		case'y':
			yy = yy + 0.1;
			glutPostRedisplay();
			break;
		case'Y':
			yy = yy - 0.1;
			glutPostRedisplay();
			break;
		case'z':
			zz = zz + 0.1;
			glutPostRedisplay();
			break;
		case'Z':
			zz = zz - 0.1;
			glutPostRedisplay();
			break;
		case 'g':// Bật hoặc tắt chế độ gạt mưa
			gatMuaEnabled = !gatMuaEnabled;
			glutPostRedisplay();
			break;
		case 'm':
			setDenXeColor(237 / 255.0, 249 / 255.0, 76 / 255.0);
			glutPostRedisplay();
			break;
		case 'M':
			setDenXeColor(207 / 255.0, 208 / 255.0, 200 / 255.0);
			glutPostRedisplay();
			break;
		case 'k':
			k = 0;
			glutPostRedisplay();
			break;
		case 'K':
			k = k + 0.1;
			if (k > 0.3)k = 0.3;
			glutPostRedisplay();
			break;
		//NUT CHO ROBOT
		case'h':
			h = h + 5;
			glutPostRedisplay();
			break;
		case'H':
			h = h - 5;
			glutPostRedisplay();
			break;
		case'j':
			j = j + 5;
			glutPostRedisplay();
			break;
		case'J':
			j = j - 5;
			glutPostRedisplay();
			break;
		case'o':
			o = o + 5;
			glutPostRedisplay();
			break;
		case'O':
			o = o - 5;
			glutPostRedisplay();
			break;
		case'p':
			p = p + 5;
			glutPostRedisplay();
			break;
		case'P':
			p = p - 5;
			glutPostRedisplay();
			break;
		//NUT DIEU CHINH QUAT
		case'0':
			speed = 0;
			glutPostRedisplay();
			break;
		case'1':
			speed = 0.1;
			glutPostRedisplay();
			break;
		case'2':
			speed = 0.3;
			glutPostRedisplay();
			break;
		case '4'://BAT TAT DEN PHONG
			la = 1.0;
			lb = 1.0;
			lc = 1.0;
			light_diffuse = color4(la, lb, lc, 1.0); 
			glutPostRedisplay();
			break;
		case '5':
			la = 0.1;
			lb = 0.1;
			lc = 0.1;
			light_diffuse = color4(la, lb, lc, 1.0); 
			glutPostRedisplay();
			break;
		}
	}    
}


int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 1000, 700 );                 
	glutInitWindowPosition(100,60);               
    glutCreateWindow( "Drawing a Cube" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
    glutKeyboardFunc( keyboard );                  

	glutMainLoop();
    return 0;
}
