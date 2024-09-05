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


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;
GLuint model_loc, view_loc;

// Các tham số cho projection
GLfloat l = -0.5, r = 0.5;
GLfloat bottom = -1, top = 1;
GLfloat zNear = 3.5, zFar = 15;
GLuint projection_loc;



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
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

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

	model_loc = glGetUniformLocation(program, "Model");
	view_loc = glGetUniformLocation(program, "view");
	projection_loc = glGetUniformLocation(program, "projection");


	glEnable(GL_DEPTH_TEST);


	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}


//Hồ Hữu Phương
GLfloat xx,yy, a, zn;
GLfloat rx, ry, rz;

mat4 model_chantru;
void chantru() {
	//chan dung 1
	mat4 instance;
	instance = Translate(0.48, 0, 0.48) * Scale(0.04, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//chan dung 2
	instance = Translate(-0.48, 0, 0.48) * Scale(0.04, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thanh ngang chan dung 1 2
	instance = Translate(0, 0.06, 0.48) * Scale(1, 0.08, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//


	//chan dung 3
	instance = Translate(0.48, 0, -0.48) * Scale(0.04, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//chan dung 4
	instance = Translate(-0.48, 0, -0.48) * Scale(0.04, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thanh ngang chan dung 3 4
	instance = Translate(0, 0.06, -0.48) * Scale(1, 0.08, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//
}

void thantru() {
	mat4 instance = Translate(0, 0.48, 0) * Scale(0.17, 0.39, 0.17) ;
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//
	instance = Translate(0, 0.06 + 0.08, 0.48) * Scale(0.1, 0.08, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//
	instance = Translate(0, 0.06 + 0.08, -0.48) * Scale(0.1, 0.08, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//ong ngang 2 chan tru
	instance = Translate(0, 0.06 + 0.08 + 0.04 + 0.05, 0) * Scale(0.1, 0.1, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void khoanglai() {
	mat4 instance;
	instance = Translate(-0.25 + 0.3 / 2, 0.76, 0) * Scale(0.5, 0.4, 0.25);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void taynang() {
	mat4 instance;

	instance = Translate(0.35, 1.4, 0) * RotateZ(70) * Scale(1.5, 0.09, 0.09);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void maykeo() {
	mat4 instance;
	//than tru may keo
	instance = Translate(0, 1.2, 0.1) * Scale(0.13, 0.8, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 1.2, -0.1) * Scale(0.13, 0.8, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 1, 0) * Scale(0.1, 0.01, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 1.2, 0) * Scale(0.1, 0.01, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 1.4, 0) * Scale(0.1, 0.01, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 1.6, 0) * Scale(0.1, 0.01, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.03, 1.6, 0.09) * RotateZ(40) * Scale(0.05, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.03, 1.6, -0.09) * RotateZ(40) * Scale(0.05, 0.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.16, 1.61, 0) * RotateZ(40) * Scale(0.5, 0.02, 0.18);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//day keo
	instance = Translate(-0.35, 1.2, 0.015) * Scale(0.01, 0.6, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.35, 1.2, -0.015) * Scale(0.01, 0.6, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.16, 1.65, 0.015) * RotateZ(40) * Scale(0.5, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-0.16, 1.65, -0.015) * RotateZ(40) * Scale(0.5, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	instance = Translate(0.32, 1.96, -0.015) * RotateZ(30) * Scale(0.75, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.32, 1.96, 0.015) * RotateZ(30) * Scale(0.75, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void rongroc() {
	mat4 instance = Translate(0.75 - 0.07, 1.7 + 0.4, 0.0f) * Scale(0.1, 0.1, 0.02) ;
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void daykeo1() {
	mat4 instance;

	//day keo 1
	instance = Translate(0.73, 1.5, 0) * Scale(0.01, 1.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//day keo 2
	instance = Translate(0.64, 1.5, 0) * Scale(0.01, 1.2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void daykeo() {
	mat4 instance;
	
	//day moc
	instance = Translate(0.685, 0.9, 0) * Scale(0.12, 0.1, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//day keo vat
	instance = Translate(0.685, 0.35, 0) * Scale(0.01, 1, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chantru * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void cantruc() {
	model_chantru = Translate(0.4, 0, 0) * Scale(0.5, 0.5, 0.5);
	chantru();
	model_chantru *= Translate(xx, 0, 0);
	thantru();
	model_chantru *= RotateX(rx) * RotateY(ry) * RotateZ(rz);
	khoanglai();
	maykeo();
	rongroc();
	taynang();
	model_chantru *= Translate(0, yy, 0);
	daykeo();
	model_chantru *= Translate(0, yy/2, 0) * Scale(1, (1.2-yy/2)/(1.2+yy/2), 1);
	daykeo1();
}


//Nguyễn Tử Nghĩa
float e, n_x;
mat4 xnanghang;
mat4 model_view;
void gamxe() {
	xnanghang = Translate(0.05, 0.0, 0) * Scale(0.7, 0.05, 0.5); //khungxe
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.03, 0.1) * Scale(0.3, 0.02, 0.03);    //treo 1
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.05, 0.1) * Scale(0.25, 0.02, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.07, 0.1) * Scale(0.20, 0.02, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.03, -0.1) * Scale(0.3, 0.02, 0.03);		//treo 2
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.05, -0.1) * Scale(0.25, 0.02, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.07, -0.1) * Scale(0.20, 0.02, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.09, 0) * Scale(0.02, 0.02, 0.5); //dong doc 1
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.09, 0.22) * Scale(0.08, 0.08, 0.08); //banh xe
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.24, -0.09, -0.22) * Scale(0.08, 0.08, 0.08); //banh xe
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//==========================================================

	xnanghang = Translate(-0.15, -0.03, 0.1) * Scale(0.3, 0.02, 0.03);    //treo 1
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, -0.05, 0.1) * Scale(0.25, 0.02, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, -0.07, 0.1) * Scale(0.20, 0.02, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, -0.03, -0.1) * Scale(0.3, 0.02, 0.03);		//treo 2
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, -0.05, -0.1) * Scale(0.25, 0.02, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, -0.07, -0.1) * Scale(0.20, 0.02, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, -0.09, 0) * Scale(0.02, 0.02, 0.5); //dong doc 1
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, -0.09, 0.22) * Scale(0.08, 0.08, 0.08); //banh xe
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, -0.09, -0.22) * Scale(0.08, 0.08, 0.08); //banh xe
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanxe() {
	xnanghang = Translate(0.2, 0.1, 0) * Scale(0.4, 0.17, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.01, 0.18, 0) * Scale(0.1, 0.02, 0.15); //ghe ngoi
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.05, 0.2, 0) * RotateZ(-10) * Scale(0.02, 0.22, 0.15);  //tua lung
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


}

void dieukhien() {
	xnanghang = RotateZ(-5) * Translate(-0.15, 0.1, 0) * Scale(0.015, 0.2, 0.015);    //can dieu khien
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = RotateZ(-5) * Translate(-0.15, 0.2, 0) * Scale(0.019, 0.015, 0.019);    //can dieu khien
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.21, 0.1, 0) * Scale(0.03, 0.2, 0.5);    //bang dieu khien
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.21, 0.295, 0) * RotateZ(-10) * Scale(0.01, 0.01, 0.07);    //vo lang
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.14, 0.285, 0) * RotateZ(-10) * Scale(0.01, 0.01, 0.07);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.17, 0.29, 0.03) * RotateZ(-10) * Scale(0.07, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.17, 0.29, -0.03) * RotateZ(-10) * Scale(0.07, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	xnanghang = Translate(-0.17, 0.29, -0.0) * RotateZ(-10) * Scale(0.035, 0.01, 0.01); //tam volang
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.19, 0.29, 0.017) * RotateZ(-10) * RotateY(45) * Scale(0.041, 0.01, 0.01); //tam volang
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.19, 0.29, -0.017) * RotateZ(-10) * RotateY(-45) * Scale(0.041, 0.01, 0.01); //tam volang
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.20, 0.19, -0) * Scale(0.02, 0.05, 0.04);   //hopso
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.19, 0.25, -0) * RotateZ(-20) * Scale(0.01, 0.08, 0.01); //noi vo lang hop so
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, 0.04, -0.06) * RotateZ(-20) * Scale(0.03, 0.01, 0.02); //chan phanh chan ga
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, 0.03, -0.06) * RotateZ(-20) * Scale(0.01, 0.03, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, 0.04, 0.06) * RotateZ(-20) * Scale(0.03, 0.01, 0.02); //chan phanh chan ga
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.15, 0.03, 0.06) * RotateZ(-20) * Scale(0.01, 0.03, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


}

void maiche() {
	xnanghang = Translate(-0.17, 0.34, 0.24) * RotateZ(-15) * Scale(0.02, 0.3, 0.02);    //thanh che truoc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.17, 0.34, -0.24) * RotateZ(-15) * Scale(0.02, 0.3, 0.02);    //thanh che truoc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.17, 0.33, -0.24) * RotateZ(5) * Scale(0.02, 0.32, 0.02);    //thanh che sau
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0.17, 0.33, 0.24) * RotateZ(5) * Scale(0.02, 0.3, 0.02);    //thanh che sau
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(0, 0.49, 0) * Scale(0.4, 0.02, 0.5);    //mai che
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.3, 0.26, -0.17) * Scale(0.03, 0.55, 0.06);    //thanh goc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang = Translate(-0.3, 0.26, 0.17) * Scale(0.03, 0.55, 0.06);    //thanh goc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

void nanghang() {
	

	xnanghang =   Translate(-0.35, 0.2, 0.22) * Scale(0.01, 0.3, 0.02);    //2doc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.2, -0.22) * Scale(0.01, 0.3, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.35, 0) * Scale(0.01, 0.02, 0.46);    //4ngang
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.25, 0) * Scale(0.01, 0.02, 0.46);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.18, 0) * Scale(0.01, 0.02, 0.46);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.06, 0) * Scale(0.01, 0.02, 0.46);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.29, 0.14) * Scale(0.01, 0.1, 0.02);  //4doc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.29, 0.05) * Scale(0.01, 0.1, 0.02);  //4doc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.29, -0.05) * Scale(0.01, 0.1, 0.02);  //4doc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.29, -0.15) * Scale(0.01, 0.1, 0.02);  //4doc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.12, -0.1) * Scale(0.01, 0.17, 0.04);  //2 ngang
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.35, 0.12, 0.1) * Scale(0.01, 0.17, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.497, 0.03, 0.1) * Scale(0.3, 0.01, 0.04);//2 doc
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.497, 0.03, -0.1) * Scale(0.3, 0.01, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.3, 0.18, -0) * Scale(0.03, 0.06, 0.34); //ket noi 
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.32, 0.18, -0) * Scale(0.05, 0.03, 0.03); //3 thanh ket noi 
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.32, 0.18, -0.1) * Scale(0.05, 0.03, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	xnanghang =   Translate(-0.32, 0.18, 0.1) * Scale(0.05, 0.03, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * xnanghang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


}

void xenanghang() {
	model_view = Translate(-0.3 + n_x, 0, 0);
	gamxe();
	thanxe();
	dieukhien();
	maiche();
	model_view *= Translate(0, e, 0);
	nanghang();
}

//Trần Tiến Cường
//phan cua tau
mat4 model, model_taus2;
mat4 model_laptau, model_tinhieutau;
mat4 model_gocnhin;
//cua tau
GLfloat tienluitau, molaptau = 0;
GLfloat quaytau, quayangtentau = 0;


//void ////DoiMau(point4 input) {//color4
//	for (int i = 0; i < NumPoints; i++) {
//		colors[i] = input;
//	}
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
//}

void thanduoi() {
	mat4 mtthan1 = Translate(0, 0, 0) * Scale(0.7, 0.2, 1.8);

	//////DoiMau(color4(1.0, 0.27, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * mtthan1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thantren() {
	mat4 mtthan2 = Translate(0, 0.125, 0) * Scale(0.6, 0.05, 1.7);

	//////DoiMau(color4(1.0, 1.0, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * mtthan2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhdai1()
{
	mat4 thanh = Translate(0.3, 0.2, 0) * Scale(0.1, 0.2, 1.8);

	//////DoiMau(color4(0.5, 0.0, 0.5, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhdai2()
{
	mat4 thanh = Translate(-0.3, 0.2, 0) * Scale(0.1, 0.2, 1.8);

	//////DoiMau(color4(0.5, 0.0, 0.5, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhngan1()
{
	mat4 thanh = Translate(0, 0.2, 0.85) * Scale(0.5, 0.2, 0.1);

	//////DoiMau(color4(0.5, 0.0, 0.5, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhngan2()
{
	mat4 thanh = Translate(0, 0.2, -0.85) * Scale(0.5, 0.2, 0.1);

	////DoiMau(color4(0.5, 0.0, 0.5, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void dauduoi() {
	mat4 dauduoi = Translate(0, 0, -0.9) * RotateY(45) * Scale(0.494, 0.2, 0.494);

	////DoiMau(color4(1.0, 0.27, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * dauduoi);

	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void dauduoi2() {
	mat4 dauduoi = Translate(0, 0, 0.9) * RotateY(45) * Scale(0.494, 0.2, 0.494);

	////DoiMau(color4(1.0, 0.27, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * dauduoi);

	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void dautren1() {
	mat4 mtthan2 = Translate(0, 0.1, -0.9) * RotateY(45) * Scale(0.353, 0.05, 0.353);

	////DoiMau(color4(1.0, 1.0, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * mtthan2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void dautren2() {
	mat4 mtthan2 = Translate(0, 0.1, 0.9) * RotateY(45) * Scale(0.353, 0.05, 0.353);

	////DoiMau(color4(1.0, 1.0, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * mtthan2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhngang1()
{
	mat4 thanh = Translate(0.34, 0.51, 0) * Scale(0.02, 0.02, 1);

	////DoiMau(color4(1.0, 0.6471, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanhngang2()
{
	mat4 thanh = Translate(-0.34, 0.51, 0) * Scale(0.02, 0.02, 1);

	////DoiMau(color4(1.0, 0.6471, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


}

void thanhdoc(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 thanh = Translate(x, y, z) * Scale(0.02, 0.21, 0.02);

	////DoiMau(color4(1.0, 0.6471, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


}


void khungthungtraiphai(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 khungthung1 = Translate(x, y, z) * Scale(0.01, 0.2, 0.4);
	////DoiMau(color4(0.1961, 0.8039, 0.1961, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * khungthung1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void khungthungtruocsau(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 khungthung1 = Translate(x, y, z) * Scale(0.5, 0.2, 0.01);
	////DoiMau(color4(0.1961, 0.8039, 0.1961, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * khungthung1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void laptau()
{
	mat4 khungthung1 = Scale(0.5, 0.01, 0.4);
	////DoiMau(color4(1.0, 0.7529, 0.7961, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model_laptau * khungthung1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanduoiangten()
{
	mat4 ngonkhoi = Translate(0, 0.25, -0.6) * Scale(0.2, 0.1, 0.2);
	////DoiMau(color4(0.0, 0.0, 1.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * ngonkhoi);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thantrenangten()
{
	mat4 ngonkhoi = Translate(0, 0.35, -0.6) * Scale(0.1, 0.05, 0.1);
	////DoiMau(color4(1.0, 0.6471, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * ngonkhoi);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void caydaiangten()
{
	mat4 ngonkhoi = Translate(0, 0.35 + 0.05, -0.6) * Scale(0.02, 0.8, 0.02);
	////DoiMau(color4(1.0, 0.0, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * ngonkhoi);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tinhieutau()
{
	mat4 ngonkhoi = Scale(0.3, 0.02, 0.02);
	////DoiMau(color4(0.8, 0.8, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model_tinhieutau * ngonkhoi);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hanghoa(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 hanghoa = Translate(x, y, z) * Scale(0.09, 0.09, 0.09);
	////DoiMau(color4(1.0, 0.0, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * hanghoa);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hanghoaxanhratroi(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 hanghoa = Translate(x, y, z) * Scale(0.09, 0.09, 0.09);
	////DoiMau(color4(0.5294, 0.8078, 0.9216, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * hanghoa);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void hanghoangngoaithuyen(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 hanghoa = Translate(x, y, z) * Scale(0.1, 0.09, 0.2);
	////DoiMau(color4(1.0, 0.0, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * hanghoa);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hanghoangngoaithuyenbac(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 hanghoa = Translate(x, y, z) * Scale(0.1, 0.09, 0.2);
	////DoiMau(color4(0.7529, 0.7529, 0.7529, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * hanghoa);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void tayvin(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 tayvinthang = Translate(x, y, z) * Scale(0.05, 0.8, 0.05);
	////DoiMau(color4(1.0, 0.0, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * tayvinthang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tayvindai(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 tayvinthang = Translate(x, y, z) * RotateX(-45) * Translate(0, 0.15, 0) * Scale(0.05, 0.3, 0.05);
	////DoiMau(color4(1.0, 0.0, 0.0, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * tayvinthang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void banhtron(GLfloat x, GLfloat z)
{
	mat4 instance;
	////DoiMau(color4(1.0, 0.27, 0.0, 1.0));
	for (int i = 0; i <= 360; i++)
	{
		instance = Translate(x, 0.15, z) * RotateZ(i) * Scale(0.7f, 0.1f, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void thantau()
{
	thantren();
	thanduoi();
}

void khungtau()
{
	thanhdai1();
	thanhdai2();
	thanhngan1();
	thanhngan2();
}

void muitau()
{
	dauduoi();
	dauduoi2();
	dautren1();
	dautren2();
}

void langcanphai()
{
	thanhngang1();//phai
	//doc phai
	thanhdoc(0.34, 0.3 + 0.105, 0);
	thanhdoc(0.34, 0.3 + 0.105, -0.20);
	thanhdoc(0.34, 0.3 + 0.105, 0.2);
	thanhdoc(0.34, 0.3 + 0.105, 0);
	thanhdoc(0.34, 0.3 + 0.105, -0.40);
	thanhdoc(0.34, 0.3 + 0.105, 0.4);
}

void langcantrai()
{

	thanhngang2();
	thanhdoc(-0.34, 0.3 + 0.105, 0);
	thanhdoc(-0.34, 0.3 + 0.105, -0.20);
	thanhdoc(-0.34, 0.3 + 0.105, 0.2);
	thanhdoc(-0.34, 0.3 + 0.105, 0);
	thanhdoc(-0.34, 0.3 + 0.105, -0.40);
	thanhdoc(-0.34, 0.3 + 0.105, 0.4);
}

void thung()
{
	khungthungtraiphai(0.25 - 0.005, 0.2, 0.6);
	khungthungtraiphai(-0.25 + 0.005, 0.2, 0.6);

	//
	khungthungtruocsau(0, 0.2, 0.4);
	khungthungtruocsau(0, 0.2, 0.8);
	//
}
void khungangten()
{
	thanduoiangten();
	thantrenangten();
	caydaiangten();

}
mat4 modelcanhquat;
void canhquat()
{
	mat4 tayvinthang = Translate(0, 0.2, 0) * Scale(0.2, 0.4, 0.05);
	////DoiMau(color4(1.0, 0.5, 0.5, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * modelcanhquat * tayvinthang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhquat1()
{
	mat4 tayvinthang = RotateZ(120) * Translate(0, 0.2, 0) * Scale(0.2, 0.4, 0.05);
	////DoiMau(color4(1.0, 0.5, 0.5, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * modelcanhquat * tayvinthang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canhquat2()
{
	mat4 tayvinthang = RotateZ(-120) * Translate(0, 0.2, 0) * Scale(0.2, 0.4, 0.05);
	////DoiMau(color4(1.0, 0.5, 0.5, 1.0));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_gocnhin * modelcanhquat * tayvinthang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void quat()
{
	canhquat();
	canhquat1();
	canhquat2();
}
void tau()
{
	thantau();
	khungtau();
	muitau();
	langcanphai();
	langcantrai();
	thung();
	khungangten();
	hanghoa(0.1, 0.2, 0.5);
	hanghoa(0.1, 0.2, 0.7);
	hanghoaxanhratroi(-0.1, 0.2, 0.5);
	hanghoaxanhratroi(-0.1, 0.2, 0.7);
	hanghoangngoaithuyen(0.1, 0.2, 0.2);
	hanghoangngoaithuyen(-0.1, 0.2, 0.2);
	hanghoangngoaithuyenbac(-0.1, 0.2, -0.1);
	hanghoangngoaithuyenbac(0.1, 0.2, -0.1);
	tayvin(0, 0.2, 1.1);
	tayvindai(0, 0.6, 1.1);
	banhtron(0, 0.95);
	banhtron(0, -0.95);


}
GLfloat quaytauquat;
void objectfulltau()
{
	model = Translate(-0.5, -0.7, tienluitau) * RotateY(quaytau) * Scale(0.5, 0.5, 0.5);
	tau();
	model_laptau = model * Translate(0, 0.3, 0.4) * RotateX(molaptau) * Translate(0, 0, 0.2);
	laptau();
	model_tinhieutau = model * Translate(0, 0.7, -0.6) * RotateY(quayangtentau);
	tinhieutau();
	modelcanhquat = model * Translate(0, 0, 1.25) * RotateZ(quaytauquat);
	quat();
}
float p, o;

//Lô Đại Lục
mat4 model_xe, model_cuatrai, model_cuaphai, model_banhxe1, model_banhxe2, model_banhtruoc;
mat4 model_nap, model_thungxe, model_nang, model_lautrai, model_lauphai;
GLfloat c = 0, b = 0;
GLfloat tienluixe = 0, mocuatrai = 0, mocuaphai = 0, banhxoay = 0, napxe = 0, thung1 = 0, nang1 = 0, nang2 = 0, lau = 0;

//void //DoiMau(point4 input) {//color4
//	for (int i = 0; i < NumPoints; i++) {
//		colors[i] = input;
//	}
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
//}

void khungduoi()
{
	mat4 duoixe = Translate(-0.45, 0.02, 0) * Scale(0.22, 0.01, 0.3);
	//DoiMau(color4(1.0, 0.27, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * duoixe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	duoixe = Translate(-0.45, 0.003, 0) * Scale(0.1, 0.03, 0.15);
	//DoiMau(color4(0.105, 0.105, 0.105, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * duoixe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//khung banh sau
	duoixe = Translate(-0.34, -0.01, -0.1125) * RotateZ(-60) * Scale(0.06, 0.01, 0.075);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * duoixe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	duoixe = Translate(-0.34, -0.01, 0.1125) * RotateZ(-60) * Scale(0.06, 0.01, 0.075);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * duoixe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	duoixe = Translate(-0.57, -0.01, 0) * RotateZ(60) * Scale(0.06, 0.01, 0.3);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * duoixe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);




}
void khunggiua()
{
	mat4  giuaxe = Translate(-0.19, 0.01, 0) * Scale(0.3, 0.03, 0.15);
	//DoiMau(color4(1.0, 0.27, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * giuaxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//dong co
	giuaxe = Translate(-0.3, -0.038, 0) * Scale(0.42, 0.01, 0.01);
	//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * giuaxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	for (int i = 0; i <= 360; i++)
	{
		giuaxe = Translate(-0.45, -0.038, 0) * RotateX(i) * Scale(0.07, 0.01, 0.05);
		//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * giuaxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	//be nang
	for (int i = 0; i <= 360; i++)
	{
		giuaxe = RotateZ(6) * Translate(-0.16, 0.075, 0) * RotateY(i) * Scale(0.025, 0.08, 0.025);
		//DoiMau(color4(0.0, 0.0, 0.0, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * giuaxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void kdauxe()
{

	//khung sau
	mat4 dauxe = Translate(-0.14, 0.085, 0) * Scale(0.01, 0.12, 0.3);
	//DoiMau(color4(1.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.14, 0.16, 0.1) * Scale(0.01, 0.03, 0.1);
	//DoiMau(color4(1.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.14, 0.16, -0.1) * Scale(0.01, 0.03, 0.1);
	//DoiMau(color4(1.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//khung truoc
	dauxe = Translate(0.035, 0.07, 0) * Scale(0.01, 0.09, 0.3);
	//DoiMau(color4(1.0, 1.0, 0.0, 0.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(0.016, 0.14, 0.145) * RotateZ(35) * Scale(0.01, 0.08, 0.01);
	//DoiMau(color4(1.0, 1.0, 0.0, 0.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(0.016, 0.14, -0.145) * RotateZ(35) * Scale(0.01, 0.08, 0.01);
	//DoiMau(color4(1.0, 1.0, 0.0, 0.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(0.045, 0.07, 0) * Scale(0.005, 0.01, 0.1);
	//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(0.045, 0.09, 0) * Scale(0.005, 0.01, 0.1);
	//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(0.045, 0.05, 0) * Scale(0.005, 0.01, 0.1);
	//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//khung tren
	dauxe = Translate(-0.075, 0.175, 0) * Scale(0.14, 0.01, 0.3);
	//DoiMau(color4(1.0, 1.0, 0.0, 0.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//khung trai
	dauxe = Translate(-0.12, 0.1, 0.1) * Scale(0.03, 0.15, 0.1);
	//DoiMau(color4(1.0, 1.0, 0.0, 0.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//khung phai
	dauxe = Translate(-0.12, 0.1, -0.1) * Scale(0.03, 0.15, 0.1);
	//DoiMau(color4(1.0, 1.0, 0.0, 0.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//vo lang
	dauxe = Translate(0.005, 0.06, 0) * Scale(0.05, 0.07, 0.2);
	//DoiMau(color4(1.39, 0.69, 0.19, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.02, 0.1, -0.08) * RotateZ(60) * Scale(0.01, 0.03, 0.01);
	//DoiMau(color4(0.65, 1.05, 2.25, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	for (int i = 0; i <= 360; i++)
	{
		dauxe = Translate(-0.03, 0.1, -0.08) * RotateZ(-20) * RotateX(i) * Scale(0.008, 0.008, 0.05);
		//DoiMau(color4(0.65, 1.05, 2.25, 1.0));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	//ghe
	dauxe = Translate(-0.1, 0.05, -0.08) * Scale(0.08, 0.05, 0.05);
	//DoiMau(color4(0.545f, 0.275f, 0.122f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.115, 0.1, -0.08) * Scale(0.05, 0.05, 0.05);
	//DoiMau(color4(0.545f, 0.275f, 0.122f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.1, 0.05, 0.08) * Scale(0.08, 0.05, 0.05);
	//DoiMau(color4(0.545f, 0.275f, 0.122f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.115, 0.1, 0.08) * Scale(0.05, 0.05, 0.05);
	//DoiMau(color4(0.545f, 0.275f, 0.122f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void cuatrai()
{
	mat4 dauxe = Translate(0.025, 0.075, -0.145) * Scale(0.01, 0.1, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuatrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(0.006, 0.14, -0.145) * RotateZ(35) * Scale(0.01, 0.065, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuatrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.06, 0.17, -0.145) * Scale(0.098, 0.01, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuatrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.095, 0.095, -0.145) * Scale(0.02, 0.14, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuatrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.033, 0.065, -0.145) * Scale(0.13, 0.08, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuatrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tay nam
	dauxe = Translate(-0.095, 0.095, -0.155) * Scale(0.01, 0.01, 0.01);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuatrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.095, 0.095, -0.135) * Scale(0.01, 0.01, 0.01);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuatrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//guong
	dauxe = Translate(0.015, 0.09, -0.165) * Scale(0.01, 0.02, 0.02);
	//DoiMau(color4(0.545f, 0.275f, 0.122f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuatrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void cuaphai()
{
	mat4 dauxe = Translate(0.025, 0.075, 0.145) * Scale(0.01, 0.1, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuaphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(0.006, 0.14, 0.145) * RotateZ(35) * Scale(0.01, 0.065, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuaphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.06, 0.17, 0.145) * Scale(0.098, 0.01, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuaphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.095, 0.095, 0.145) * Scale(0.02, 0.14, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuaphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.033, 0.065, 0.145) * Scale(0.13, 0.08, 0.01);
	//DoiMau(color4(0.0, 1.0, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuaphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tay nam
	dauxe = Translate(-0.095, 0.095, 0.155) * Scale(0.01, 0.01, 0.01);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuaphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.095, 0.095, 0.135) * Scale(0.01, 0.01, 0.01);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuaphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//guong
	dauxe = Translate(0.015, 0.09, 0.165) * Scale(0.01, 0.02, 0.02);
	//DoiMau(color4(0.545f, 0.275f, 0.122f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_cuaphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void khungdau()
{
	mat4 dauxe = Scale(0.08, 0.05, 0.3);
	//DoiMau(color4(1.0, 0.27, 0.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//den
	for (int i = 0; i <= 360; i++)
	{
		dauxe = Translate(0.045, 0, -0.1) * RotateX(i) * Scale(0.005, 0.01, 0.03);
		//DoiMau(color4(0.7529f, 0.7529f, 0.7529f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	for (int i = 0; i <= 360; i++)
	{
		dauxe = Translate(0.045, 0, 0.1) * RotateX(i) * Scale(0.005, 0.01, 0.03);
		//DoiMau(color4(0.7529f, 0.7529f, 0.7529f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	//khung  banh dau
	//truoc
	dauxe = Translate(-0.05, -0.01, -0.1125) * RotateZ(-60) * Scale(0.06, 0.01, 0.075);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.05, -0.01, 0.1125) * RotateZ(-60) * Scale(0.06, 0.01, 0.075);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// tren 
	dauxe = Translate(-0.1, 0.02, -0.1125) * Scale(0.08, 0.01, 0.075);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.1, 0.02, 0.1125) * Scale(0.08, 0.01, 0.075);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// sau
	dauxe = Translate(-0.15, -0.01, -0.1125) * RotateZ(60) * Scale(0.06, 0.01, 0.075);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	dauxe = Translate(-0.15, -0.01, 0.1125) * RotateZ(60) * Scale(0.06, 0.01, 0.075);
	//DoiMau(color4(0.0, 0.0, 1.0, 1.0));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_xe * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	kdauxe();

}
void banhxesau1()
{
	mat4 duoixe;
	for (int i = 0; i <= 360; i++)
	{
		duoixe = Translate(-0.4, -0.038, -0.1125) * RotateZ(i) * Scale(0.001, 0.1, 0.04);
		//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe1 * duoixe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	for (int i = 0; i <= 360; i++)
	{
		duoixe = Translate(-0.4, -0.038, 0.1125) * RotateZ(i) * Scale(0.001, 0.1, 0.04);
		//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe1 * duoixe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	//thanh ngang
	duoixe = Translate(-0.4, -0.038, 0) * Scale(0.01, 0.01, 0.2);
	//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe1 * duoixe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//vien banh
	//banh trai
	for (int i = 0; i <= 360; i++)
	{
		if (i % 60 == 0)
		{
			duoixe = Translate(-0.4, -0.038, -0.1335) * RotateZ(i) * Scale(0.01, 0.06, 0.001);
			//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe1 * duoixe);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		}
	}
	for (int i = 0; i <= 360; i++)
	{


		duoixe = Translate(-0.4, -0.038, -0.1335) * RotateZ(i) * Translate(0, -0.035, 0) * Scale(0.01, 0.01, 0.001);
		//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe1 * duoixe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}
	//banh phai
	//banh trai
	for (int i = 0; i <= 360; i++)
	{
		if (i % 60 == 0)
		{
			duoixe = Translate(-0.4, -0.038, 0.1335) * RotateZ(i) * Scale(0.01, 0.06, 0.001);
			//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe1 * duoixe);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		}
	}
	for (int i = 0; i <= 360; i++)
	{


		duoixe = Translate(-0.4, -0.038, 0.1335) * RotateZ(i) * Translate(0, -0.035, 0) * Scale(0.01, 0.01, 0.001);
		//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe1 * duoixe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}
}
void banhxesau2()
{
	mat4 duoixe;
	for (int i = 0; i <= 360; i++)
	{
		duoixe = Translate(-0.51, -0.038, -0.1125) * RotateZ(i) * Scale(0.001, 0.1, 0.04);
		//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe2 * duoixe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	for (int i = 0; i <= 360; i++)
	{
		duoixe = Translate(-0.51, -0.038, 0.1125) * RotateZ(i) * Scale(0.001, 0.1, 0.04);
		//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe2 * duoixe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	//thanh ngang
	duoixe = Translate(-0.51, -0.038, 0) * Scale(0.01, 0.01, 0.2);
	//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe2 * duoixe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//vien banh
	//banh trai
	for (int i = 0; i <= 360; i++)
	{
		if (i % 60 == 0)
		{
			duoixe = Translate(-0.51, -0.038, -0.1335) * RotateZ(i) * Scale(0.01, 0.06, 0.001);
			//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe2 * duoixe);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		}
	}
	for (int i = 0; i <= 360; i++)
	{


		duoixe = Translate(-0.51, -0.038, -0.1335) * RotateZ(i) * Translate(0, -0.035, 0) * Scale(0.01, 0.01, 0.001);
		//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe2 * duoixe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}
	//banh phai
	for (int i = 0; i <= 360; i++)
	{
		if (i % 60 == 0)
		{
			duoixe = Translate(-0.51, -0.038, 0.1335) * RotateZ(i) * Scale(0.01, 0.06, 0.001);
			//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe2 * duoixe);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		}
	}
	for (int i = 0; i <= 360; i++)
	{


		duoixe = Translate(-0.51, -0.038, 0.1335) * RotateZ(i) * Translate(0, -0.035, 0) * Scale(0.01, 0.01, 0.001);
		//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhxe2 * duoixe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}
}
void banhxetruoc()
{
	mat4 dauxe;
	for (int i = 0; i <= 360; i++)
	{
		dauxe = Translate(-0.1, -0.038, -0.1125) * RotateZ(i) * Scale(0.001, 0.1, 0.04);
		//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhtruoc * dauxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	for (int i = 0; i <= 360; i++)
	{
		dauxe = Translate(-0.1, -0.038, 0.1125) * RotateZ(i) * Scale(0.001, 0.1, 0.04);
		//DoiMau(color4(0.0, 0.0, 0.0, 1.0));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhtruoc * dauxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	//thanh ngang
	dauxe = Translate(-0.1, -0.038, 0) * Scale(0.01, 0.01, 0.2);
	//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhtruoc * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//vien banh xe
	//banh trai
	for (int i = 0; i <= 360; i++)
	{
		if (i % 60 == 0)
		{
			dauxe = Translate(-0.1, -0.038, -0.1335) * RotateZ(i) * Scale(0.01, 0.06, 0.001);
			//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhtruoc * dauxe);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		}
	}
	for (int i = 0; i <= 360; i++)
	{


		dauxe = Translate(-0.1, -0.038, -0.1335) * RotateZ(i) * Translate(0, -0.035, 0) * Scale(0.01, 0.01, 0.001);
		//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhtruoc * dauxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}
	//banh phai
	for (int i = 0; i <= 360; i++)
	{
		if (i % 60 == 0)
		{
			dauxe = Translate(-0.1, -0.038, 0.1335) * RotateZ(i) * Scale(0.01, 0.06, 0.001);
			//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
			glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhtruoc * dauxe);
			glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		}
	}
	for (int i = 0; i <= 360; i++)
	{


		dauxe = Translate(-0.1, -0.038, 0.1335) * RotateZ(i) * Translate(0, -0.035, 0) * Scale(0.01, 0.01, 0.001);
		//DoiMau(color4(0.4f, 0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_banhtruoc * dauxe);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	}


}
void thungxe()
{
	mat4 thungxe = Translate(-0.2, 0.12, 0) * Scale(0.01, 0.19, 0.3);
	//DoiMau(color4(0.0f, 0.5f, 0.0f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_thungxe * thungxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	thungxe = Translate(-0.4, 0.1, 0.145) * Scale(0.4, 0.155, 0.01);
	//DoiMau(color4(0.0f, 0.5f, 0.0f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_thungxe * thungxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	thungxe = Translate(-0.4, 0.1, -0.145) * Scale(0.4, 0.155, 0.01);
	//DoiMau(color4(0.0f, 0.5f, 0.0f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_thungxe * thungxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	thungxe = Translate(-0.4, 0.03, 0) * Scale(0.4, 0.01, 0.29);
	//DoiMau(color4(0.545f, 0.275f, 0.122f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_thungxe * thungxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	thungxe = Translate(-0.17, 0.205, 0) * Scale(0.05, 0.02, 0.3);
	//DoiMau(color4(0.0f, 0.5f, 0.0f, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_thungxe * thungxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


}
void cuathung()
{
	mat4 thungxe = Translate(-0.6, 0.1, 0) * Scale(0.01, 0.15, 0.29);
	//DoiMau(color4(1.0, 0.0, 1.0, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_nap * thungxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void benang()
{
	mat4 nang;
	//nang

	//nang1
	for (int i = 0; i <= 360; i++)
	{
		nang = RotateZ(6) * Translate(-0.16, 0.15, 0) * RotateY(i) * Scale(0.02, 0.1 + nang1, 0.02);
		//DoiMau(color4(0.4, 0.4, 0.4, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_nang * nang);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
	//nang2
	for (int i = 0; i <= 360; i++)
	{
		nang = RotateZ(6) * Translate(-0.16, 0.2 + nang1 / 2 + nang2 / 2, 0) * RotateY(i) * Scale(0.015, 0.03 + nang2, 0.015);
		//DoiMau(color4(0.75f, 0.75f, 0.75f, 1.0f));
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_nang * nang);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

}
void laukinhtrai()
{
	mat4 dauxe;

	dauxe = Translate(0.03, 0.12, 0.07) * RotateZ(35) * Scale(0.005, 0.005, 0.06);
	//DoiMau(color4(0, 0, 0, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_lautrai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


}
void laukinhphai()
{
	mat4 dauxe;

	dauxe = Translate(0.03, 0.12, -0.07) * RotateZ(35) * Scale(0.005, 0.005, 0.06);
	//DoiMau(color4(0, 0, 0, 1.0f));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_lauphai * dauxe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void khungxe()
{
	khungduoi();
	khunggiua();
	khungdau();

}
void fullxe()
{
	model_xe = Translate(0.2 + tienluixe, -0.5, 0);
	khungxe();
	model_cuatrai = model_xe * Translate(0.03, 0, -0.145) * RotateY(mocuatrai) * Translate(-0.03, 0, 0.145);
	cuatrai();
	model_cuaphai = model_xe * Translate(0.03, 0, 0.145) * RotateY(mocuaphai) * Translate(-0.03, 0, -0.145);
	cuaphai();
	model_banhtruoc = model_xe * Translate(-0.1, -0.038, 0) * RotateZ(banhxoay) * Translate(0.1, 0.038, 0);
	banhxetruoc();
	model_banhxe1 = model_xe * Translate(-0.4, -0.038, 0) * RotateZ(banhxoay) * Translate(0.4, 0.038, 0);
	banhxesau1();
	model_banhxe2 = model_xe * Translate(-0.51, -0.038, 0) * RotateZ(banhxoay) * Translate(0.51, 0.038, 0);
	banhxesau2();
	model_thungxe = model_xe * Translate(-0.55, 0.1, 0) * RotateZ(thung1) * Translate(0.55, -0.1, 0);
	thungxe();
	model_nap = model_thungxe * Translate(-0.6, 0.175, 0) * RotateZ(napxe) * Translate(0.6, -0.175, 0);
	cuathung();
	model_nang = model_xe * RotateZ(6) * Translate(0, nang1 / 2, 0) * RotateZ(-6);
	benang();
	model_lautrai = model_xe * Translate(0.03, 0.12, 0.045) * RotateX(lau) * Translate(-0.03, -0.12, -0.045);
	laukinhtrai();
	model_lauphai = model_xe * Translate(0.03, 0.12, -0.095) * RotateX(lau) * Translate(-0.03, -0.12, 0.095);
	laukinhphai();
}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4 eye = RotateY(p) * vec4(1, 1, 5 + o, 1);
	vec4 at = vec4(0, 0, 0, 1);
	vec4 up = vec4(0, 1, 0, 1);

	mat4 v = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, v);

	mat4 p = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);

	cantruc();
	xenanghang();
	objectfulltau();
	model = RotateX(a) * RotateY(b) * RotateZ(c) * Translate(0.5, 0, 0) * Scale(1.5, 1.5, 1.5);
	fullxe();

	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
	//Phuong
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	case '1':
		ry = ry + 10;
		glutPostRedisplay();
		break;
	case '2':
		ry = ry - 10;
		glutPostRedisplay();
		break;
	case '3':
		if (yy <= 0.5) {
			yy = yy + 0.05;
		}
		glutPostRedisplay();
		break;
	case '4':
		if (yy >= 0) {
			yy = yy - 0.05;
		}
		glutPostRedisplay();
		break;
	case '5':
		if (xx > -0.45) {
			xx = xx - 0.05;
		}
		glutPostRedisplay();
		break;
	case '6':
		if (xx < 0.45) {
			xx = xx + 0.05;
		}
		glutPostRedisplay();
		break;
	//Eye
	case '[':
		p += 1;
		glutPostRedisplay();
		break;
	case ']':
		p -= 1;
		glutPostRedisplay();
		break;
	case'\\':
		o -= 0.05;
		glutPostRedisplay();
		break;

	case'|':
		o += 0.05;
		glutPostRedisplay();
		break;
	//Nghia
	case'n':
		e += 0.01;
		if (e >= 0.34) e = 0.34;

		glutPostRedisplay();
		break;

	case'h':
		e -= 0.01;
		if (e <= -0.15) e = -0.15;

		glutPostRedisplay();
		break;
	case'H':
		n_x -= 0.01;

		glutPostRedisplay();
		break;
	case'N':
		if(n_x < 0)
			n_x += 0.01;
	

		glutPostRedisplay();
		break;
	//Cuong
	case 'a':
		tienluitau += 0.1;//tientauto

		glutPostRedisplay();
		break;
	case 'A':
		tienluitau -= 0.1;//luitauto

		glutPostRedisplay();
		break;
	case 'B':
		quaytau += 5;
		glutPostRedisplay();//quaysang trai tau to
		break;
	case 'b':
		quaytau -= 5;
		glutPostRedisplay();//quaysangphai tau to
		break;
	case 'c':
		quaytauquat -= 15;// quay quat di chuyen
		tienluitau -= 0.05;
		glutPostRedisplay();
		break;
	case 'C':
		quaytauquat += 15;
		tienluitau += 0.05;
		glutPostRedisplay(); //quay quat cua tau di chuyen
		break;

	case 'D':
		molaptau += 0.8;
		if (molaptau >= 0) molaptau = 0;//MO LAP TAU
		glutPostRedisplay();
		break;
	case 'd':
		molaptau -= 0.8;
		if (molaptau <= -180) molaptau = -180;// DONG LAP TAU
		glutPostRedisplay();
		break;
	case 'F':
		quayangtentau -= 5;
		glutPostRedisplay();//QUAY ANG TEN SANG PHAI
		break;
	case 'f':
		quayangtentau += 5;
		glutPostRedisplay();// QUAY ANG TEN SANG TRAI
		break;
	//Luc
	case 't':
		glutIdleFunc([]() {
			tienluixe += 0.005;
			banhxoay -= 10;
			if (banhxoay <= -360)
				banhxoay += 360;
			glutPostRedisplay();
			});
		break;
	case 'x':
		glutIdleFunc(NULL);
		break;
	case 'T':
		glutIdleFunc([]() {
			tienluixe -= 0.005;
			banhxoay += 10;
			if (banhxoay >= 360)
				banhxoay -= 360;
			glutPostRedisplay();
			});
		break;
	case ',':
		mocuatrai -= 10;
		if (mocuatrai <= -80)
			mocuatrai = -80;
		glutPostRedisplay();
		break;
	case '<':
		mocuatrai += 10;
		if (mocuatrai >= 0)
			mocuatrai = 0;
		glutPostRedisplay();
		break;
	case 'm':
		mocuaphai += 10;
		if (mocuaphai >= 80)
			mocuaphai = 80;
		glutPostRedisplay();
		break;
	case 'M':
		mocuaphai -= 10;
		if (mocuaphai <= 0)
			mocuaphai = 0;
		glutPostRedisplay();
		break;
	case 'q':
		napxe -= 5;
		if (napxe <= -90)
			napxe = -90;
		glutPostRedisplay();
		break;
	case 'Q':
		napxe += 5;
		if (napxe >= 0)
			napxe = 0;
		glutPostRedisplay();
		break;
	case 'p':
		nang1 += 0.03;
		thung1 += 3;
		if (nang1 >= 0.06)
		{
			nang1 = 0.06;
			nang2 += 0.015;
			if (nang2 >= 0.045)
				nang2 = 0.045;
		}
		if (thung1 >= 15)
			thung1 = 15;
		glutPostRedisplay();
		break;
	case 'P':
		nang2 -= 0.02;
		thung1 -= 3;
		if (nang2 <= 0)
		{
			nang1 -= 0.03;
			nang2 = 0;

			if (nang1 <= 0)
				nang1 = 0;
		}
		if (thung1 <= 0)
			thung1 = 0;
		glutPostRedisplay();
		break;
	case 'l':
		lau -= 10;
		if (lau <= -150)
			lau = -150;
		glutPostRedisplay();
		break;

	case 'L':
		lau += 10;
		if (lau >= 0)
			lau = 0;
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
	glutCreateWindow("Drawing a Cube");


	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}
