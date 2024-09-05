//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/

typedef vec4 point4;
typedef vec4 color4;
using namespace std;
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
const int soDinhHinhTru = 144;
int Index = 0;
point4 points[NumPoints + soDinhHinhTru]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints + soDinhHinhTru]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints + soDinhHinhTru]; /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/
point4 DinhHLP[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 MauHLP[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/
point4 DinhHtru[26]; GLfloat dr = M_PI / 180;
color4 MauHtru[26];
#pragma endregion Các biến chức năng
#pragma region 
int on = 0;
GLfloat light_color[] = { 1.0, 1.0, 1.0 };
GLfloat light[3] = { 74, 73, 67 };
color4 light_ambient = color4(1.0, 1.0, 1.0, 1.0);
color4 light_diffuse = color4(1.0, 1.0, 1.0, 1.0);
color4 light_specular = color4(1.0, 1.0, 1.0, 1.0);
point4 light_position = point4(-7, 4.2, 4.6, 1.0);
color4 material_ambient(0.2, 0.2, 0.2, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1.0, 1.0, 1.0, 1.0);


float material_shininess = 100;
#pragma endregion Các biến chiếu sáng

vec4 eye(0, 3, -30, 1), at(0, 0, 0, 1), up(0, 1, 0, 1);
GLfloat l = -0.01, r = 0.01, b = -0.01, t = 0.01, zN = 0.02, zF = 50;
mat4 view_mat;
mat4 move_mat;
mat4 model_view;
GLfloat matXY[2]; // doc ngang
GLuint  light_a_loc, light_d_loc, light_s_loc, light_pos_loc, light_shine_loc;
vec4 mat_dinhhuong(0, 0, 1, 1);
GLfloat speedTranslate = 5;
GLfloat speedRotate = 5;
#pragma endregion Các biến điều chỉnh eye


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;
GLuint model_view_loc, view_loc, projection_loc;

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
	vertex_colors[6] = color4(1.0, 0.2, 1.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 n = normalize(cross(u, v));
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

void TaoDinhHtru()
{
	//hình trụ mặt trên
	int j = 0;
	for (int i = 0; i <= 11; i++) {
		DinhHtru[i] = point4(cos(j * dr), 1, sin(j * dr), 1.0);
		j = j + 30;
	}
	DinhHtru[12] = point4(0, 1, 0, 1.0);
	//mặt dưới
	j = 0;
	for (int i = 13; i <= 24; i++) {
		DinhHtru[i] = point4(cos(j * dr), -1, sin(j * dr), 1.0);
		j = j + 30;
	}
	DinhHtru[25] = point4(0, -1, 0, 1.0);
}
void MBHTru(int a, int b, int c, int d) {
	vec4 u = DinhHtru[b] - DinhHtru[a];
	vec4 v = DinhHtru[c] - DinhHtru[b];
	vec3 n = normalize(cross(u, v));
	normals[Index] = n; points[Index] = DinhHtru[a]; Index++;
	normals[Index] = n; points[Index] = DinhHtru[b]; Index++;
	normals[Index] = n; points[Index] = DinhHtru[c]; Index++;

	normals[Index] = n; points[Index] = DinhHtru[a]; Index++;
	normals[Index] = n; points[Index] = DinhHtru[c]; Index++;
	normals[Index] = n; points[Index] = DinhHtru[d]; Index++;
}
void MTHTru1(int a[]) {
	vec4 u = DinhHtru[a[1]] - DinhHtru[a[7]];
	vec4 v = DinhHtru[a[4]] - DinhHtru[a[10]];
	vec3 n = normalize(cross(u, v));
	for (int i = 2; i <= 12; i++) {
		normals[Index] = n; points[Index] = DinhHtru[a[0]]; Index++;
		normals[Index] = n; points[Index] = DinhHtru[a[i - 1]]; Index++;
		normals[Index] = n; points[Index] = DinhHtru[a[i]]; Index++;
		if (i == 12) {
			normals[Index] = n; points[Index] = DinhHtru[a[0]]; Index++;
			normals[Index] = n; points[Index] = DinhHtru[a[12]]; Index++;
			normals[Index] = n; points[Index] = DinhHtru[a[1]]; Index++;
		}
	}
}
void MTHTru2(int a[]) {
	vec4 u = DinhHtru[a[1]] - DinhHtru[a[7]];
	vec4 v = DinhHtru[a[4]] - DinhHtru[a[10]];
	vec3 n = -1 * normalize(cross(u, v));
	for (int i = 2; i <= 12; i++) {
		normals[Index] = n; points[Index] = DinhHtru[a[0]]; Index++;
		normals[Index] = n; points[Index] = DinhHtru[a[i - 1]]; Index++;
		normals[Index] = n; points[Index] = DinhHtru[a[i]]; Index++;
		if (i == 12) {
			normals[Index] = n; points[Index] = DinhHtru[a[0]]; Index++;
			normals[Index] = n; points[Index] = DinhHtru[a[12]]; Index++;
			normals[Index] = n; points[Index] = DinhHtru[a[1]]; Index++;
		}
	}
}
void HinhTru() {
	int a[] = { 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int b[] = { 25, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	//mặt tròn
	MTHTru2(a);
	MTHTru1(b);
	//thân hình trụ
	for (int i = 1; i <= 11; i++) {
		MBHTru(a[i], a[i + 1], b[i + 1], b[i]);
	}
	MBHTru(a[12], a[1], b[1], b[12]);
}
void VeHinhTru(mat4 instance, mat4 matrix)
{
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_view * matrix * instance);
	glDrawArrays(GL_TRIANGLES, NumPoints, soDinhHinhTru);    /*Vẽ các tam giác*/
}
void VeHinhHop(mat4 instance, mat4 matrix) {
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_view * matrix * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void generateGeometry(void)
{
	//Vẽ hình lập phươngs
	initCube();
	makeColorCube();

	//Vẽ hình trụ 
	TaoDinhHtru();
	HinhTru();
}
float Nitpick(int a) {
	return (float)a / 255;
}
color4 ConvertRGB(int a, int b, int c) {
	return color4(Nitpick(a), Nitpick(b), Nitpick(c), 1.0);
}

void FillColor(vec4 color) {
	material_ambient = color;
	material_diffuse = color;
	material_specular = color;
	material_shininess = 100;
	if (on == 1) {
		light[0] = 255;
		light[1] = 255;
		light[2] = 255;
	}
	else if (on == -1) {
		light[0] = 227;
		light[1] = 223;
		light[2] = 163;
	}
	else {
		light[0] = 74;
		light[1] = 73;
		light[2] = 67;
	}
	light_ambient = ConvertRGB(light[0], light[1], light[2]);
	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
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

	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	light_a_loc = glGetUniformLocation(program, "AmbientLight");
	light_d_loc = glGetUniformLocation(program, "DiffuseLight");
	light_s_loc = glGetUniformLocation(program, "SpecularLight");
	light_pos_loc = glGetUniformLocation(program, "LightPosition");
	model_view_loc = glGetUniformLocation(program, "Model_View");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");
	glEnable(GL_DEPTH_TEST);
	vec4 color = ConvertRGB(135, 206, 235);
	glClearColor(color.x, color.y, color.z, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}



mat4 instance  ;
mat4  model_tang1 , model_kdc ;
GLfloat gocquaycua[] = { 0,0 };
GLfloat theta[] = { 0, 0, 0 };
GLfloat theta1[] = { 0, 0, 0 };
GLfloat theta2[] = { 0, 0, 0 };
GLfloat phong[] = { 0, 0, 0 };
GLfloat goc = 0 ,xcrv = 0.0;
GLfloat chibi[] = { 0,0,0 };
// Thiết kế tòa nhà 
//Tòa chính 
#pragma region 
// Khung phòng 
void Mat_Ben(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(181, 126, 220);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(60.0, 12.0, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Mat_Ben2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(204, 78, 92);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(60.0, 12.0, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Mat_TrenDuoi(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(245, 245, 220);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * RotateX(90) * Scale(60.11, 30.0, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Liner(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 0, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * RotateX(90) * Scale(60.0, 0.1, 0.07);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Liner2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 0, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * RotateX(90) * Scale(0.02, 30, 0.07);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void Mat_Sau(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(192, 192, 192);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * RotateY(90) * Scale(30.0, 12.0, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Mat_Ngang(GLfloat x, GLfloat y, GLfloat z, GLfloat a,GLfloat b, GLfloat c)
{
	material_diffuse = ConvertRGB(a,b,c);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * RotateY(90) * Scale(30.0, 3.3, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void Mat_Doc(GLfloat x, GLfloat y, GLfloat z, GLfloat a, GLfloat b, GLfloat c)
{
	material_diffuse = ConvertRGB(a,b,c);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * RotateY(90) * Scale(11.75, 9.0, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void Cua_Ravao() {
	material_diffuse = ConvertRGB(231, 213, 222);
	FillColor(material_diffuse);
	mat4 instance = Translate(16.2, 0, 0) * Scale(0.1, 9.2, 6.6) * Translate(0,0,xcrv);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Mat_Truoc(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(192, 192, 192);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * RotateY(90) * Scale(30.0, 12.0, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void TrangTri(GLfloat x, GLfloat y, GLfloat z) {
	mat4 model_tt = model_view * Translate(x, y, z);
	Liner(-16.8, 7.3, 2);
	Liner(-16.8, 7.3, -2);
	Liner(-16.8, 7.3, 6);
	Liner(-16.8, 7.3, -6);
	Liner(-16.8, 7.3, 10);
	Liner(-16.8, 7.3, -10);
	Liner2(0, 7.3, 0);
	Liner2(4, 7.3, 0);
	Liner2(8, 7.3, 0);
	Liner2(12, 7.3, 0);
	Liner2(-4, 7.3, 0);
	Liner2(-8, 7.3, 0);
	Liner2(-12, 7.3, 0);
	Liner2(-16, 7.3, 0);
	Liner2(-20, 7.3, 0);
	Liner2(-24, 7.3, 0);
	Liner2(-28, 7.3, 0);
	Liner2(-32, 7.3, 0);
	Liner2(-36, 7.3, 0);
	Liner2(-40, 7.3, 0);
}


void Mat_Truoc1() {
	Mat_Ngang(-37, 6, 0, 101, 116, 76);
	Mat_Doc(-37, 0, 9, 101, 116, 76);
	Mat_Doc(-37, 0, -9, 101, 116, 76);
}

void Mat_Truoc2() {
	Mat_Ngang(16, 6, 0, 0, 3, 128);
	Mat_Doc(16, 0, 9, 0, 0, 128);
	Mat_Doc(16, 0, -9, 0, 0, 128);
	Cua_Ravao();
}

void Tang(GLfloat x, GLfloat y, GLfloat z)
{
	model_tang1 = model_view * Translate(x, y, z);
	// Sảnh trước 
	Mat_Ben(-13.75, 1.5, -15);
	Mat_Ben(-13.75, 1.5, 15);
	Mat_Ben2(-13.75, 1.5, -15.01);
	Mat_Ben2(-13.75, 1.5, 15.01);
	Mat_TrenDuoi(-13.8, -4.5, 0);
	Mat_TrenDuoi(-13.8, 7.5, 0);
	TrangTri(0, 0, 0);
	Mat_Sau(-43.75, 1.5, 0);
	Mat_Truoc1();


}

void MainBuilding() {
	Tang(0, 0, 0);
	Mat_Truoc2();
	Tang(0, 12, 0);
	Mat_Truoc(15, 1.8, 0);
}
#pragma endregion 

// Duy Anh 
#pragma region 
// Tủ để giày
#pragma region 
mat4 model_tu, model_quaytu;
void Mat_Ben_Tu(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(150, 111, 51);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(3.5, 6.5, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Thanh_Ngang(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(150, 111, 51);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(3.4, 0.2, 4.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Mat_Sau_Tu(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(128, 128, 128);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.2, 6.55, 4.55);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Canh_Tu(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(255, 255, 255);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.2, 3.1, 2.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_quaytu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void CaiTu(GLfloat x, GLfloat y, GLfloat z,GLfloat goc)
{
	model_tu = model_view * RotateY(goc) * Translate(x, y, z) * Translate(theta2[0], 0, theta2[1]) * RotateY(theta2[2]);
	Mat_Ben_Tu(0, 0, 2.2);
	Mat_Ben_Tu(0, 0, -2.2);
	Mat_Ben_Tu(0, 0, 0);
	Thanh_Ngang(0, 0, 0);
	Thanh_Ngang(0, 1.6, 0);
	Thanh_Ngang(0, 3.2, 0);
	Thanh_Ngang(0, -3.2, 0);
	Mat_Sau_Tu(-1.8, 0, 0);
	model_quaytu = model_tu * Translate(1.75, 0, 2.16) * RotateY(theta[0]) * Translate(-1.75, 0, -2.16);
	Canh_Tu(1.66, -1.64, 1.08);
	model_quaytu = model_tu * Translate(1.75, 0, -2.16) * RotateY(theta[1]) * Translate(-1.75, 0, 2.16);
	Canh_Tu(1.66, -1.64, -1.08);
}
#pragma endregion 
//Thang máy
#pragma region
mat4  model_tm, model_tm_2;
void khungtm() {
	material_diffuse = ConvertRGB(111, 222, 123);
	FillColor(material_diffuse);

	//mặt trên
	mat4 instance = Translate(0, 3.9, 0) * Scale(6.4, 0.3, 5.8);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tm * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mặt dưới 
	instance = Translate(0, -4, 0) * Scale(6.4, 0.3, 5.8);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tm * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mặt trái
	material_diffuse = ConvertRGB(211, 122, 123);
	FillColor(material_diffuse);
	instance = Translate(-3.2, 0, 0) * Scale(0.3, 8, 5.8);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tm * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mặt phải
	instance = Translate(3.2, 0, 0) * Scale(0.3, 8, 5.8);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tm * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mặt sau
	material_diffuse = ConvertRGB(121, 212, 123);
	FillColor(material_diffuse);
	instance = Translate(0, 0, -1.2) * Scale(6.4, 8, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tm * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canh1() {
	material_diffuse = ConvertRGB(231, 213, 222);
	FillColor(material_diffuse);
	mat4 instance = Translate(-1.7, 2.5, 0) * Scale(3, 8, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tm * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void canh2() {
	material_diffuse = ConvertRGB(231, 213, 222);
	FillColor(material_diffuse);
	mat4 instance = Translate(1.7, 2.5, 0) * Scale(3, 8, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_tm_2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	material_diffuse = ConvertRGB(115, 98, 65);
	FillColor(material_diffuse);
}
GLfloat zbd, cua1_ad, cua2_ad, z3d;
GLfloat xtm = 0.0 , updown =0.0;

void ThangMay() {
	model_tm =  Translate(0, updown, 40) * RotateY(180) ;
	khungtm();

	model_tm_2 = model_tm;
	model_tm = model_tm * Translate(3.2, -2.6, 3) * Translate(cua1_ad,0,0);
	canh1();

	model_tm_2 = model_tm_2 * Translate(-3.2, -2.6, 3) * Translate(cua2_ad, 0, 0);
	canh2();
}
#pragma endregion
// Quầy thu ngân 
#pragma region 
mat4 model_maytinh, model_maytinh1, model_maytinh2, model_thanhcheo1, model_thanhcheo2, model_thanhcheo3, model_thanhcheo4 , model_thanhdoc,model_keMT;
mat4 model_bthngan;
GLfloat qq[] = { 0,0,0 };
// Máy thanh toán
#pragma region
void MT_De(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 231, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(2, 0.1, 0.8);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_maytinh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT_ThanhDoc(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 231, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.4, 1, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_maytinh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT_ThanhCheo1(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 0, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.2, 1, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_thanhcheo1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT_ThanhCheo2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 0, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.2, 0.7, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_thanhcheo2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT_ThanhCheo3(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 0, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.2, 1, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_thanhcheo3 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT_ThanhCheo4(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 0, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.2, 0.7, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_thanhcheo4 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT1_ManHinh(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 231, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(1.94, 1.2, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_maytinh1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT1_1(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 0, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.03, 1.2, 0.13);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_maytinh1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT1_2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 231, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(2, 0.03, 0.13);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_maytinh1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MayTinh1()
{
	MT1_ManHinh(0, 0, 0);
	MT1_1(0.98, 0, 0);
	MT1_1(-0.98, 0, 0);
	MT1_2(0, 0.6, 0);
	MT1_2(0, -0.6, 0);
}

void MT2_ManHinh(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 231, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(1.94, 1.2, 0.1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_maytinh2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT2_1(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 0, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.03, 1.2, 0.13);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_maytinh2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MT2_2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 231, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(2, 0.03, 0.13);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_maytinh2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MayTinh2()
{
	MT2_ManHinh(0, 0, 0);
	MT2_1(0.98, 0, 0);
	MT2_1(-0.98, 0, 0);
	MT2_2(0, 0.6, 0);
	MT2_2(0, -0.6, 0);

}

GLfloat model_thanhcheo, thanhcheo1, thanhcheo2;
void ThanhCheo1()
{
	MT_ThanhCheo1(-2.4, 0.4, 1.4);
	MT_ThanhCheo2(-2.4, 0.9, 1.4);
}

void ThanhCheo2()
{
	MT_ThanhCheo3(-2.4, -2, 0);
	MT_ThanhCheo4(-2.4, -2.6, 0);
}

void KhungMT()
{
	MT_De(-2.4, 0.3, 1);
	MT_ThanhDoc(-2.4, 0.82, 1);
}

void MayTinh()
{
	model_maytinh = model_bthngan  * Translate(chibi[0],chibi[1],chibi[2]) ;
	KhungMT();
	model_thanhcheo1 = model_maytinh * RotateX(-45);
	model_thanhcheo2 = model_thanhcheo1 * Translate(0, 0, 0);
	ThanhCheo1();

	model_thanhcheo3 = model_maytinh * RotateX(225);
	model_thanhcheo4 = model_thanhcheo3 * Translate(0, 0, 0);
	ThanhCheo2();

	model_maytinh1 = model_thanhcheo2 * Translate(-2.2, 1.3, 1.5) * RotateX(33) * RotateX(45);
	MayTinh1();

	model_maytinh2 = model_thanhcheo4 * Translate(-2.2, -3, 0) * RotateX(-25) * RotateX(-225);
	MayTinh2();

}
#pragma endregion
// Bàn thu ngân
#pragma region
mat4 model_bantinhtien, model_ngankeo2, model_ngankeo1, model_ngankeo, model_ngankeo3;

void Chan(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(255, 255, 255);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(6, 4.0, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_bantinhtien * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Chan2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(255, 255, 255);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(8.6, 4.0, 0.5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_bantinhtien * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Chan3(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 0, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.1, 4.0, 8.4);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_bantinhtien * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MatBan(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(66, 231, 218);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(3.5, 0.5, 9.6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_bantinhtien * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MatBan2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 231, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(0.2, 0.2, 9.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,  model_bantinhtien * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MatBan3(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 231, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(3.2, 0.2, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,   model_bantinhtien * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void KhungBan()
{
	model_bantinhtien = model_bthngan * RotateY(-90) ;
	Chan(-1.35, -2.2, -4.2);
	Chan2(-2.6, -2.2, 4.2);
	Chan3(1.7, -2.18, 0);
	MatBan(0, 0, 0);
	MatBan2(-1.55, 0.17, 0);
	MatBan2(1.55, 0.17, 0);
	MatBan3(-0.02, 0.17, 4.5);
	MatBan3(-0.02, 0.17, -4.5);
}

void BanThuNgan()
{
	model_bthngan =  RotateY(-90)  * Translate(qq[0],0,qq[2]) *Translate(-8, 0, -8) *RotateY(qq[1])  ;
	MayTinh();
	KhungBan();
}

#pragma endregion
#pragma endregion


#pragma endregion
// Phúc 
#pragma region 

mat4 model_ban, model_ghe1, model_ghe2, model_ghe;
mat4 ctm3;
GLfloat znk = 0;

void matban(GLfloat x, GLfloat y, GLfloat z)    /*vtri thanh rao co toa do (x,y,z)*/
{
	material_diffuse = ConvertRGB(203, 92, 141);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(5, 0.1, 3.0);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctm3 * instance);

	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}


void chanban(GLfloat x, GLfloat y, GLfloat z)    /*vtri thanh rao co toa do (x,y,z)*/
{
	material_diffuse = ConvertRGB(111, 111, 111);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(0.4, 1.6, 0.4);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctm3 * instance);

	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}



void base()
{
	matban(0, 0.5, 0);

	chanban(-2, -0.4, 1);
	chanban(-2, -0.4, -1);
	chanban(2, -0.4, -1);
	chanban(2, -0.4, 1);
}

void thanhngang(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(111, 111, 111);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(2, 0.5, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctm3 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhdoc(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(111, 111, 111);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(0.2, 0.5, 1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctm3 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void dayngankeo(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(111, 111, 111);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(2, 0.2, 1);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctm3 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ngankeo() {
	thanhngang(0, 0, 1.5);
	thanhngang(0, 0, 0.5);
	thanhdoc(0.8, 0, 1);
	thanhdoc(-0.8, 0, 1);
	dayngankeo(0, 0, 1);
}

void matghe(GLfloat x, GLfloat y, GLfloat z) {
	mat4 instance = Translate(x, y, z) * Scale(1.8, 0.3, 2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_ghe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void changhe(GLfloat x, GLfloat y, GLfloat z) {
	mat4 instance = Translate(x, y, z) * Scale(0.2, 1.2, 0.2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_ghe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void lungghe(GLfloat x, GLfloat y, GLfloat z) {
	mat4 instance = Translate(x, y, z) * Scale(0.1, 1.5, 2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_ghe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void ghe()   /*vtri thanh rao co toa do (x,y,z)*/
{
	matghe(-3, 0, 0);
	lungghe(-3.9, 0.8, 0);
	changhe(-2.2, -0.6, -0.8);
	changhe(-2.2, -0.6, 0.8);
	changhe(-3.8, -0.6, -0.8);
	changhe(-3.8, -0.6, 0.8);

}
GLfloat x1 = 0, z1 = 0;
GLfloat x2 = 0, z2 = 0;
GLfloat xb = 0, zb = 0;
GLfloat a = 0;


void banhoc(GLfloat x, GLfloat y, GLfloat z)
{
	ctm3 = Translate(x,y,z) * Translate(xb, -3, zb);
	base();
	ctm3 = ctm3 * Translate(0, 0, znk);
	ngankeo();

}
void veghe() {
	model_ghe = Translate(x1, -3, z1);
	ghe();
}
mat4 ctm4, model_sofa, model_goisf1, model_goisf2;
GLfloat xsf = 0, zsf = 0, xg1 = 0, xg2 = 0;

void matduoi(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(336, 240, 148);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(5, 1, 3);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm4 * model_sofa * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tualung(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(255, 209, 220);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(5, 2, 0.5);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm4 * model_sofa * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuatay(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(255, 255, 255);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(0.5, 0.75, 2);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm4 * model_sofa * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void goisf1(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(255, 155, 245);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(1.2, 0.5, 0.5);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm4 * model_goisf2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void goisf2(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(255, 155, 245);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(1.2, 0.5, 0.5);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm4 * model_goisf1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void sofa() {
	matduoi(0, -1, 0);
	tualung(0, 0, 1.25);
	tuatay(2.25, 0, 0);
	tuatay(-2.25, 0, 0);
}
GLfloat sophia;
void bosofa(GLfloat x, GLfloat y, GLfloat z) {
	ctm4 = RotateY(90) * Translate(x, y, z);
	model_sofa = Translate(xsf, 0, zsf) * RotateY(sophia);
	sofa();
	model_goisf1 = model_sofa * Translate(xg1, 0, 0);
	model_goisf2 = model_sofa * Translate(xg2, 0, 0);
	goisf1(1, -0.15, 0.75);
	goisf2(-1, -0.15, 0.75);
}
#pragma endregion 
// Đạt 
#pragma region 
//tủ đầu giường
mat4 m, m_ngan_keo, m_canh_tu;
GLfloat NKTuGiong = 0, aaa = 0, quay_tu_giuong = 0;
GLfloat xxx = 0, yyy = 0, zzz = 0;
//đáy tủ
void DayTu(GLfloat x) {
	material_diffuse = ConvertRGB(141, 94, 47);
	FillColor(material_diffuse);
	mat4 instance = Translate(0, x, 0) * Scale(3, 0.15, 3);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
//thành tủ
void ThanhTu(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
	material_diffuse = ConvertRGB(114, 73, 53);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * RotateY(a) * Scale(3, 4.5, 0.15);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
//chân tủ
void ChanTu(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(114, 73, 53);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(0.3, 4.5, 0.3);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
//tay cầm cánh tủ
void TayCam(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
	material_diffuse = ConvertRGB(232, 227, 221);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * RotateZ(a) * Scale(0.15, 0.75, 0.3);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * m_canh_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
//cánh tủ
void CanhTu()
{
	material_diffuse = ConvertRGB(189, 142, 74);
	FillColor(material_diffuse);
	mat4 instance = Translate(0, 0.75, -1.5) * Scale(3, 3, 0.15);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * m_canh_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void ThanTu()
{
	DayTu(0);
	DayTu(0.9);
	DayTu(1.8);
	DayTu(3.6);// 
	ThanhTu(0, 1.5, 1.5, 0);
	ThanhTu(1.5, 1.5, 0, 90);
	ThanhTu(-1.5, 1.5, 0, 90);
	ChanTu(1.425, 1.275, 1.425);
	ChanTu(-1.425, 1.275, -1.395);
	ChanTu(-1.425, 1.275, 1.425);
	ChanTu(1.425, 1.275, -1.395);
}
//ngăn kéo trên
void ThanhNganKeoTuG(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(114, 73, 53);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(0.15, 0.6, 3);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * m_ngan_keo * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void DayTu1(GLfloat x) {
	material_diffuse = ConvertRGB(141, 94, 47);
	FillColor(material_diffuse);
	mat4 instance = Translate(0, x, 0) * Scale(3, 0.15, 3);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * m_ngan_keo * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void MatNganKeo()
{
	material_diffuse = ConvertRGB(234, 191, 124);
	FillColor(material_diffuse);
	mat4 instance = Translate(0, 3, -1.5) * Scale(3, 1.5, 0.15);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * m_ngan_keo * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void TayCam1(GLfloat x, GLfloat y, GLfloat z, GLfloat a)
{
	material_diffuse = ConvertRGB(232, 227, 221);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * RotateZ(a) * Scale(0.15, 0.75, 0.3);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, m * m_ngan_keo * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void NganKeoTuGiuong()
{
	DayTu1(2.7);
	MatNganKeo();
	TayCam1(0, 3.3, -1.65, 90);
	ThanhNganKeoTuG(1.2, 3, 0);
	ThanhNganKeoTuG(-1.2, 3, 0);
}
void canh_tu_dau_giuong()
{
	CanhTu();
	TayCam(-0.75, 1.5, -1.65, 0);
}
void tu(GLfloat x, GLfloat y, GLfloat z) {
	m = model_view * Translate(x, y, z) * Translate(xxx, yyy, zzz) * RotateY(quay_tu_giuong);
	ThanTu();
	m_ngan_keo = Translate(0, 0, NKTuGiong);
	NganKeoTuGiuong();
	m_canh_tu = Translate(1.5, 0, -1.5) * RotateY(-aaa) * Translate(-1.5, 0, 1.5);
	canh_tu_dau_giuong();
}

// Tủ quẩn áo
mat4 m2, m_canh_phai, m_canh_trai;
GLfloat anpha = 0, beta = 0, quay_tu_qao = 0, xxxx = 0, yyyy = 0, zzzz = 0;
void canhthantu(GLfloat x, GLfloat y, GLfloat z, GLfloat anpha) {
	material_diffuse = ConvertRGB(73, 45, 37);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * RotateY(anpha) * Scale(3.5, 4.9, 0.21);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, m2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void daytu(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(114, 73, 53);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(3.08, 0.21, 3.08);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, m2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhngang() {
	material_diffuse = ConvertRGB(232, 227, 221);
	FillColor(material_diffuse);
	instance = Translate(0, 3.5, -1.4) * Scale(3.08, 0.21, 0.21);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, m2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thantu() {
	canhthantu(0, 2.45, 0, 0);
	canhthantu(1.645, 2.45, -1.645, 90);
	canhthantu(-1.645, 2.45, -1.645, 90);
	daytu(0, 0.105, -1.645);
	daytu(0, 4.795, -1.645);
	thanhngang();
}
void canhtuphai() {
	material_diffuse = ConvertRGB(232, 227, 221);
	FillColor(material_diffuse);
	mat4 instance = Translate(0.77, 2.45, -3.29) * Scale(1.54, 4.9, 0.21);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, m2 * m_canh_phai * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void num_phai() {
	material_diffuse = ConvertRGB(114, 73, 53);
	FillColor(material_diffuse);
	instance = Translate(0.14, 2.45, -3.5) * Scale(0.21, 0.7, 0.21);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, m2 * m_canh_phai * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
//canhtrai
void canhtutrai() {
	material_diffuse = ConvertRGB(232, 227, 221);
	FillColor(material_diffuse);
	mat4 instance = Translate(-0.77, 2.45, -3.29) * Scale(1.54, 4.9, 0.21);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, m2 * m_canh_trai * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}//-0.11, 0.35, -0.47
void num_trai() {
	material_diffuse = ConvertRGB(114, 73, 53);
	FillColor(material_diffuse);
	instance = Translate(-0.14, 2.45, -3.5) * Scale(0.21, 0.7, 0.21);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, m2 * m_canh_trai * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhphai() {
	num_phai();
	canhtuphai();
};
void canhtrai() {
	canhtutrai();
	num_trai();
}
void vetu(GLfloat x, GLfloat y, GLfloat z) {
	m2 = model_view * Translate(x, y, z) * Translate(xxxx, yyyy, zzzz) * RotateY(quay_tu_qao) * Translate(0, 0, 1.75);
	thantu();
	m_canh_phai = Translate(1.54, 0, -3.29) * RotateY(-anpha) * Translate(-1.54, 0, 3.29);
	canhphai();
	m_canh_trai = Translate(-1.54, 0, -3.29) * RotateY(beta) * Translate(1.54, 0, 3.29);
	canhtrai();
}

#pragma endregion 
// Hồng Anh
#pragma region 
// Hồng Anh
// Hồng Anh
mat4 ctm;
mat4 model_giuong, model_goi1, model_goi2;

GLfloat xoay_demY = 0, xoay_demX = 0, goi1 = 0, goi2 = 0;
GLfloat  zz = 0, yy = 0, xx = 0, aa = 0, di_chuyen_demX = 0, di_chuyen_demZ = 0, di_chuyen_demY = 0;

void ThanGiuong()
{
	material_diffuse = ConvertRGB(233, 150, 122);
	FillColor(material_diffuse);
	mat4 instance = Translate(0, 0, 0) * Scale(2.4, 0.6, 2.4);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm * model_giuong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ChanGiuong(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(124, 252, 0);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(0.15, 0.75, 0.15);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm * model_giuong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ThanhGiuong(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 255, 255);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(2.4, 0.6, 0.15);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm * model_giuong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void DemGiuong()
{
	material_diffuse = ConvertRGB(255, 0, 255);
	FillColor(material_diffuse);
	mat4 instance = Translate(0, 0.375, 0) * Scale(2.28, 0.15, 2.28);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm * model_giuong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Goi1(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 0, 0);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(0.75, 0.15, 0.45);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm * model_goi1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Goi2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(107, 142, 35);
	FillColor(material_diffuse);
	mat4 instance = Translate(x, y, z) * Scale(0.75, 0.15, 0.45);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, ctm * model_goi2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void caiTham(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(48, 80, 80);
	FillColor(material_diffuse);
	mat4 instance = RotateY(90) * Translate(x,y,z) * Scale(4, 0.1, 10);
	glUniformMatrix4fv(model_view_loc, GL_TRUE, 1, model_tang1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void KhungGiuong()
{

	ThanGiuong();
	ThanhGiuong(0, 0.45, 1.2);
	ChanGiuong(1.12, -0.15, 1.2);
	ChanGiuong(-1.12, -0.15, 1.2);
	ChanGiuong(1.12, -0.15, -1.2);
	ChanGiuong(-1.12, -0.15, -1.2);
}
void Giuong(GLfloat x, GLfloat y, GLfloat z, GLfloat goc)
{
	ctm = model_view * Translate(x, y, z) * RotateY(goc);
	model_giuong = Translate(xx + 1.2, yy + 0.6, zz - 3.6) * Scale(2.7) * RotateY(aa - 90);
	KhungGiuong();
	model_giuong = model_giuong * Translate(di_chuyen_demX, di_chuyen_demY, di_chuyen_demZ) * Translate(0, 0.3, -di_chuyen_demZ - 1.2) * RotateX(xoay_demX) * Translate(0, -0.3, di_chuyen_demZ + 1.2) * RotateY(xoay_demY);
	DemGiuong();
	model_goi1 = model_giuong * Translate(0, 0, goi1);
	model_goi2 = model_giuong * Translate(0, 0, goi2);
	Goi1(0.6, 0.525, 0.75);
	Goi2(-0.6, 0.525, 0.75);

}
void Tang2() {
	Giuong(-20, 8.5, 12, 90);
	Giuong(-10, 8.5, 12, 90);
	Giuong(0, 8.5, 12, 90);
	Giuong(10, 8.5, 12, 90);
	Giuong(-20, 8.5, -12, -90);
	Giuong(-10, 8.5, -12, -90);
	Giuong(0, 8.5, -12, -90);
	Giuong(-30, 8.5, -12, -90);
	caiTham(0, -4.2,-9);
}
#pragma endregion 

// Ngoại Cảnh
#pragma region 

mat4 model_duong;

//Con đường 
#pragma region
void MatDuong(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(231, 231, 231);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(300, 0.2, 200);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_duong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void ViaHe(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(255, 215, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(300, 0.2, 100);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_duong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

#pragma endregion
// Vạch kẻ đường
#pragma region 
mat4 model_vk1, model_vk2;
void VachKe1(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(0, 0, 0);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(300, 0.1, 5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE,model_vk1 * model_duong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void VachKe2(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = ConvertRGB(255, 255, 255);
	FillColor(material_diffuse);
	instance = Translate(x, y, z) * Scale(20, 0.1, 2);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_vk2 * model_duong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void PhanLan(GLfloat x, GLfloat y, GLfloat z) {
	model_vk2 = Translate(x, y, z);
	VachKe2(0, -4.5, -65);
	VachKe2(30, -4.5, -65);
	VachKe2(60, -4.5, -65);
	VachKe2(90, -4.5, -65);
	VachKe2(120, -4.5, -65);
	VachKe2(-30, -4.5, -65);
	VachKe2(-60, -4.5, -65);
	VachKe2(-90, -4.5, -65);
	VachKe2(-120, -4.5, -65);
}

void VachKeDuong() {
	VachKe1(0, -4.5, -35);
	VachKe1(0, -4.5, -95);
	VachKe1(0, -4.5, -65);
	PhanLan(0,0, 14);
	PhanLan(0, 0, -14);
}
#pragma endregion 
mat4 model_denduong;

// Cây 
#pragma region 
mat4 model_cay;
// Ngọn cây
void ngoncay(float x, float y, float z, float a, float b, float c)
{
	material_diffuse = ConvertRGB(48, 153, 32);
	FillColor(material_diffuse);
	for (int i = 0; i <= 360; i++)
	{
		instance = Translate(x, y, z) * Scale(a, b, c);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_cay * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

// Thân cây
void thancay(float x, float y, float z)
{
	material_diffuse = ConvertRGB(117, 87, 18);
	FillColor(material_diffuse);
	for (int i = 0; i <= 360; i++)
	{
		instance = RotateY(i) * Scale(1.25, 9, 1.25);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_cay * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}



void caicay(float x, float y, float z, GLfloat goc)
{
	model_cay = Translate(x, y, z); *Scale(0.4, 0.4, 0.4);
	thancay(0, 0, 0);
	model_cay *=  Translate(0, 5, 0);
	ngoncay(0, -2, 0, 6, 3, 6);
	ngoncay(0, -0.5, 0, 4, 2, 4);
	ngoncay(0, 0.5, 0, 2, 2, 2);
}

void HangCay(GLfloat z) {
	caicay(-20, 0, z,0);
	caicay(20, 0, z,0);
	caicay(35, 0, z, 0);
	caicay(50, 0, z, 0);
	caicay(65, 0, z, 0);
	caicay(-35, 0, z, 0);
	caicay(-50, 0, z, 0);
	caicay(-65, 0, z, 0);
	caicay(80, 0, z, 0);
	caicay(95, 0, z, 0);
	caicay(110, 0, z, 0);
	caicay(125, 0, z, 0);
	caicay(140, 0, z, 0);
	caicay(-80, 0, z, 0);
	caicay(-95, 0, z, 0);
	caicay(-110, 0, z, 0);
	caicay(-125, 0, z, 0);
	caicay(-140, 0, z, 0);
}

#pragma endregion 
// Đèn Đường
#pragma region 
void DenDuong(GLfloat x, GLfloat y, GLfloat z, GLfloat sxoay)
{
	model_denduong = Translate(x, y, z) * RotateY(sxoay);

	material_diffuse = ConvertRGB(152, 172, 165);
	FillColor(material_diffuse);
	for (int i = 0; i <= 360; i++)
	{
		instance = Translate(0, 6.5, -34) * RotateY(i) * Scale(2, 22, 2);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_denduong * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	material_diffuse = ConvertRGB(0, 0, 0);
	FillColor(material_diffuse);
	for (int i = 0; i <= 360; i++)
	{
		instance = Translate(0, 15.8, -36) * RotateX(45) * RotateZ(i) * Scale(1, 1, 6);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_denduong * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	for (int i = 0; i <= 360; i++)
	{
		instance = Translate(0, 15.8, -32)* RotateY(180) * RotateX(45) * RotateZ(i) * Scale(1, 1, 6);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_denduong * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}

	instance = Translate(0, 17.7, -38.7) * Scale(4, 0.5, 6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_denduong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 17.4, -38.7) * Scale(2, 0.5, 4);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_denduong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	instance = Translate(0, 17.7, -29) * Scale(4, 0.5, 6);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_denduong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 17.4, -29) * Scale(2, 0.5, 4);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_denduong * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Den(GLfloat z) {
	DenDuong(30, 0, z, 0);
	DenDuong(60, 0, z, 0);
	DenDuong(90, 0, z, 0);
	DenDuong(120, 0, z, 0);
	DenDuong(-30, 0, z, 0);
	DenDuong(-60, 0, z, 0);
	DenDuong(-90, 0, z, 0);
	DenDuong(-120, 0, z, 0);
}
#pragma endregion 
//Xe cộ 
#pragma region 
mat4 model_xe1, model_xe2, model_xe3, model_xe4, model_banh1, model_banh2, model_banh3, model_banh4;
void khungxe(mat4 model) {
	material_diffuse = ConvertRGB(32, 63, 132);
	FillColor(material_diffuse);
	mat4 instance = Scale(9.6, 2, 5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thungxe(mat4 model) {
	material_diffuse = ConvertRGB(89, 101, 97);
	FillColor(material_diffuse);
	mat4 instance = Translate(1.3, 2.5, 0) * Scale(7, 3, 5);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void banh1(mat4 model) {

	for (int i = 0; i <= 360; i++)
	{
		material_diffuse = ConvertRGB(0, 0, 0);
		FillColor(material_diffuse);
		mat4 instance = RotateZ(i) * Scale(1.5, 1.5, 0.5);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_banh1 * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);

		material_diffuse = ConvertRGB(255, 255, 255);
		FillColor(material_diffuse);
		instance = RotateZ(i) * Scale(1, 0.5, 0.5);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_banh1 * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void banh2(mat4 model) {


	for (int i = 0; i <= 360; i++)
	{
		material_diffuse = ConvertRGB(0, 0, 0);
		FillColor(material_diffuse);
		mat4 instance = RotateZ(i) * Scale(1.5, 1.5, 0.5);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_banh2 * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		material_diffuse = ConvertRGB(255, 255, 255);
		FillColor(material_diffuse);
		instance = RotateZ(i) * Scale(1, 0.5, 0.5);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_banh2 * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void banh3(mat4 model) {

	for (int i = 0; i <= 360; i++)
	{
		material_diffuse = ConvertRGB(0, 0, 0);
		FillColor(material_diffuse);
		mat4 instance = RotateZ(i) * Scale(1.5, 1.5, 0.5);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_banh3 * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		material_diffuse = ConvertRGB(255, 255, 255);
		FillColor(material_diffuse);
		instance = RotateZ(i) * Scale(1, 0.5, 0.5);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_banh3 * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}
void banh4(mat4 model) {

	for (int i = 0; i <= 360; i++)
	{
		material_diffuse = ConvertRGB(0, 0, 0);
		FillColor(material_diffuse);
		mat4 instance = RotateZ(i) * Scale(1.5, 1.5, 0.5);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_banh4 * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
		material_diffuse = ConvertRGB(255, 255, 255);
		FillColor(material_diffuse);
		instance = RotateZ(i) * Scale(1, 0.5, 0.5);
		glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_banh4 * model * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	}
}

GLfloat xxe, xxc,xxn,xxm, b1;
void xe1() {
	model_xe1 = Translate(20, -2, -95) * Translate(xxe, 0, 0);
	khungxe(model_xe1);
	thungxe(model_xe1);

	model_banh1 = model_xe1 * Translate(-2.5, -1, 2) * RotateZ(b1) * Translate(-20, 2, 95) * Translate(-xxe, 0, 0);
	banh1(model_xe1);

	model_banh2 = model_xe1 * Translate(2.5, -1, 2) * RotateZ(b1) * Translate(-20, 2, 95) * Translate(-xxe, 0, 0);
	banh2(model_xe1);

	model_banh3 = model_xe1 * Translate(-2.5, -1, -2) * RotateZ(b1) * Translate(-20, 2, 95) * Translate(-xxe, 0, 0);
	banh3(model_xe1);

	model_banh4 = model_xe1 * Translate(2.5, -1, -2) * RotateZ(b1) * Translate(-20, 2, 95) * Translate(-xxe, 0, 0);
	banh4(model_xe1);
}

void xe2() {
	model_xe2 = Translate(20, -2, -55) * Translate(xxc, 0, 0) * RotateY(180);
	khungxe(model_xe2);
	thungxe(model_xe2);

	model_banh1 = model_xe2 * Translate(-2.5, -1, 2) * RotateZ(b1) * Translate(-20, 2, 55) * Translate(-xxc, 0, 0);
	banh1(model_xe2);

	model_banh2 = model_xe2 * Translate(2.5, -1, 2) * RotateZ(b1) * Translate(-20, 2, 55) * Translate(-xxc, 0, 0);
	banh2(model_xe2);

	model_banh3 = model_xe2 * Translate(-2.5, -1, -2) * RotateZ(b1) * Translate(-20, 2, 55) * Translate(-xxc, 0, 0);
	banh3(model_xe2);

	model_banh4 = model_xe2 * Translate(2.5, -1, -2) * RotateZ(b1) * Translate(-20, 2, 55) * Translate(-xxc, 0, 0);
	banh4(model_xe2);
}

void xe3() {
	model_xe3 = Translate(20, -2, -65) * Translate(xxn, 0, 0) ;
	khungxe(model_xe3);
	thungxe(model_xe3);

	model_banh1 = model_xe3 * Translate(-2.5, -1, 2) * RotateZ(b1) * Translate(-20, 2, 65) * Translate(-xxn, 0, 0);
	banh1(model_xe3);

	model_banh2 = model_xe3 * Translate(2.5, -1, 2) * RotateZ(b1) * Translate(-20, 2, 65) * Translate(-xxn, 0, 0);
	banh2(model_xe3);

	model_banh3 = model_xe3 * Translate(-2.5, -1, -2) * RotateZ(b1) * Translate(-20, 2, 65) * Translate(-xxn, 0, 0);
	banh3(model_xe3);

	model_banh4 = model_xe3 * Translate(2.5, -1, -2) * RotateZ(b1) * Translate(-20, 2, 65) * Translate(-xxn, 0, 0);
	banh4(model_xe3);
}

void xe4() {
	model_xe4 = Translate(20, -2, -85) * Translate(xxm, 0, 0) * RotateY(180);
	khungxe(model_xe4);
	thungxe(model_xe4);

	model_banh1 = model_xe4 * Translate(-2.5, -1, 2) * RotateZ(b1) * Translate(-20, 2, 85) * Translate(-xxm, 0, 0);
	banh1(model_xe4);

	model_banh2 = model_xe4 * Translate(2.5, -1, 2) * RotateZ(b1) * Translate(-20, 2, 85) * Translate(-xxm, 0, 0);
	banh2(model_xe4);

	model_banh3 = model_xe4 * Translate(-2.5, -1, -2) * RotateZ(b1) * Translate(-20, 2, 85) * Translate(-xxm, 0, 0);
	banh3(model_xe4);

	model_banh4 = model_xe4 * Translate(2.5, -1, -2) * RotateZ(b1) * Translate(-20, 2, 85) * Translate(-xxm, 0, 0);
	banh4(model_xe4);
}

void XeCo() {
	xe1();
	xe2();
	xe3();
	xe4();
}
#pragma endregion 
// Mây 
#pragma region
mat4 model_may;
float xmay;

void Taomay() {
	material_diffuse = ConvertRGB(255, 255, 255);
	FillColor(material_diffuse);
	mat4 m =  Translate(0, 40, 15) * Scale(5, 1, 3);
	VeHinhHop(m, model_may);
	m = Translate(0.75 + 1.5, 20, 15 + 0.5) * Scale(1.5, 1, 1);
	VeHinhHop(m, model_may);
	m = Translate(5, 20, 18) * Scale(3, 0.1, 2);
	VeHinhHop(m, model_may);
	m = Translate(0.75 + 1.5 + 5, 20, 15 + 0.5 + 3) * Scale(1.5, 1, 1);
	VeHinhHop(m, model_may);

	m = Translate(4, 0, 4) * Translate(0, 20, 15) * Scale(3, 1, 2);
	VeHinhHop(m, model_may);
	m = Translate(4, 0, 4) * Translate(0.75 + 1.5, 20, 15 + 0.5) * Scale(1.5, 1, 1);
	VeHinhHop(m, model_may);
	m = Translate(5, 20, 18) * Scale(3, 1, 2);
	VeHinhHop(m, model_may);
	m = Translate(0.75 + 1.5 + 5, 20, 15 + 0.5 + 3) * Scale(1.5, 0.1, 1);
	VeHinhHop(m, model_may);
}

void VeMay() {
	Taomay();
	model_may = RotateY(90) * Translate(-14 + xmay, 0, 7) * Scale(0.8, 1, 0.8);
	Taomay();
	model_may = RotateY(90) * Translate(8 + xmay, 0, 5) * Scale(1.1, 1, 1.1);
	Taomay();
	model_may = RotateY(90) * Translate(-4 + xmay, 0, 5) * Scale(1.1, 1, 1.1);
	Taomay();
	model_may = RotateY(90) * Translate(8 + xmay, 0, 5) * Scale(1.1, 1, 1.1);
	Taomay();
	model_may = RotateY(90) * Translate(-4 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();
	model_may = RotateY(90) * Translate(-24 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();
	model_may = RotateY(90) * Translate(18 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();
	model_may = RotateY(90) * Translate(14 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();
	model_may = RotateY(90) * Translate(-14 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();
	model_may = RotateY(90) * Translate(-18 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();

	model_may = RotateY(90) * Translate(-12 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();

	model_may = RotateY(90) * Translate(-16 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();

	model_may = RotateY(90) * Translate(12 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();

	model_may = RotateY(90) * Translate(16 + xmay, 0, 0) * Scale(0.6, 1, 0.6);
	Taomay();
}
void May() {
	model_may = model_view * Translate(-60, 50, 15);
	VeMay();
}

#pragma endregion
void idle() {
	//Xe đi lại 
	xxe -= 2.5;
	xxn -= 1;
	if (xxe < -100) xxe = 80;
	if (xxn < -100)  xxn = 80;
	b1 += 80;

	xxc += 2;
	xxm += 1.5;
	if (xxc > 100) xxc = -80;
	if (xxm > 100) xxm = -80;
	glutPostRedisplay();
	//Mây trôi
	 xmay += 0.5f;
	if (xmay > 100) xmay = -100;
	//Thang máy 
	bool check = false ;
	//updown += 0.5;
	if (updown > 10) updown = 24.1;
}
bool increasing = true;

void timer(int value) 
{
	const float increment = 0.1;
	const float limit = 1.5;
	const float epsilon = 0.1; // Sai số nhỏ

	if (increasing) {
		xcrv += increment;
		if (xcrv >= limit - epsilon) { // Kiểm tra với epsilon
			increasing = false;
		}
	}
	else {
		xcrv -= increment;
		if (xcrv <= 0.0 + epsilon) { // Kiểm tra với epsilon
			increasing = true;
		}
	}

	// Kiểm tra giới hạn trước khi gọi lại hàm timer
	if (xcrv > 0.0 && xcrv < limit) {
		glutTimerFunc(400, timer, 0); // Lặp lại sau mỗi 100ms
	}

	glutPostRedisplay();
}


void NgoaiCanh(GLfloat x, GLfloat y, GLfloat z)
{
	model_duong = model_view * RotateY(-90) * Translate(x, y, z);
	MatDuong(0, -5.3, -22);
	ViaHe(0, -5.2, 15);
	ViaHe(0, -5.2, -145);
	VachKeDuong();
	Den(-41);
	HangCay(-25);
	HangCay(-125);
	May();
	XeCo();
}
#pragma endregion 
void Eye()
{
	// -------------------------Tạo mắt nhìn--------------------------
	vec4 at = eye + normalize(mat_dinhhuong);
	eye.w = at.w = 1;

	mat4 view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	// -------------------------Tạo khung nhìn--------------------------
	mat4 projection = Frustum(l, r, b, t, zN, zF);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model_view = RotateY(90);
	Eye();
	MainBuilding();
	//Duy Anh
	CaiTu(-13, -2, 16,-90);
	ThangMay();
	BanThuNgan();
	// Đạt
	tu(-12, -4.6, 13);
	vetu(-6, -5.2, 13);
	//Hồng Anh
	Tang2();
	//Phúc
	banhoc(7, 0 , 30);
	bosofa(-30, -3, 12);
	// 
	NgoaiCanh(0, 0, -10);
	glutSwapBuffers();
}

void MouseClick(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON: on = 0; glutPostRedisplay(); break;
		case GLUT_MIDDLE_BUTTON: on = 1; glutPostRedisplay(); break;
		case GLUT_RIGHT_BUTTON: on = -1; glutPostRedisplay(); break;
		}
	}
}

bool duyanhControl = false;
bool phucControl = false;
bool datControl = false;
bool honganhControl = false;

void keyboard(unsigned char key, int x, int y)
{
	if (duyanhControl == false || phucControl == false || datControl == false || honganhControl == false) {
		switch (key) {
		case '$':
			duyanhControl = true;
			phucControl = false;
			datControl = false;
			honganhControl = false;
			break;
		case 'P':
			duyanhControl = false;
			phucControl = true;
			datControl = false;
			honganhControl = false;
			break;
		case 'N':
			duyanhControl = false;
			phucControl = false;
			datControl = true;
			honganhControl = false;
			break;
		case 'H':
			duyanhControl = false;
			phucControl = false;
			datControl = false;
			honganhControl = true;
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
		case ';':
			matXY[1] -= speedRotate;
			if (matXY[1] >= 360) matXY[1] -= 360;
			if (matXY[1] < 0) matXY[1] += 360;
			break;
		case ':':
			matXY[1] += speedRotate;
			if (matXY[1] >= 360) matXY[1] -= 360;
			if (matXY[1] < 0) matXY[1] += 360;
			break;
		case '=':
			matXY[0] -= speedRotate;
			if (matXY[0] < -75) {
				matXY[0] = -75;
			}
			break;
		case '+':
			matXY[0] += speedRotate;
			if (matXY[0] > 75) {
				matXY[0] = 75;
			}
			break;
		}
	}
	/// Duy Anh
	if (duyanhControl == true) {
		switch (key)
		{
			//Tủ để giày 
		case'l':
			theta[0] -= 5;
			if (theta[0] < -180)  theta[0] = -180;
			break;
		case'L':
			theta[0] += 5;
			if (theta[0] > 0)  theta[0] = 0;
			break;
		case'r':
			theta[1] += 5;
			if (theta[1] > 180)  theta[1] = 180;
			break;
		case'R':
			theta[1] -= 5;
			if (theta[1] < 0)  theta[1] = 0;
			break;
		case'x':
			theta2[0] -= 2;
			break;
		case'X':
			theta2[0] += 2;
			break;
		case'c':
			theta2[1] -= 2;
			break;
		case'C':
			theta2[1] += 2;
			break;
		case'z':
			theta2[2] -= 5;
			break;
		case'Z':
			theta2[2] += 5;
			break;
			//mở cửa thang máy
		case '/':
			cua1_ad +=  0.25;
			if (cua1_ad > 4) cua1_ad = 4;
			cua2_ad -=  0.25;
			if (cua2_ad < -4) cua2_ad = -4;
			break;
			//đóng cửa thang máy
		case '?':
				cua1_ad -=  0.25;
				if (cua1_ad < 0) cua1_ad = 0;
				cua2_ad += 0.25;
				if (cua2_ad > 0) cua2_ad = 0;
			break;
			// Thang máy lên xuống
		case'^':
			updown += 0.5;
			break;
		case'6':
			updown -= 0.5;
			break;
			// Bàn thu ngân 
		case'v':
			qq[0] += 0.5;
			break;
		case'V':
			qq[0] -= 0.5;
			break;
		case'b':
			qq[2] += 0.5;
			break;
		case'B':
			qq[2] -= 0.5;
			break;
		case'm':
			qq[1] += 5;
			break;
		case'M':
			qq[1] -= 5;
			break;
			//Máy tính 
		case'1':
			chibi[0] += 0.5;
			break;
		case'!':
			chibi[0] -= 0.5;
			break;
		case'2':
			chibi[1] += 0.5;
			if (chibi[1] > 5) chibi[1] = 5;
			break;
		case'@':
			chibi[1] -= 0.5;
			if (chibi[1] < 0) chibi[1] = 0;
			break;
		case'3':
			chibi[2] += 0.5;
			break;
		case'#':
			chibi[2] -= 0.5;
			break;

		}
	}
	// Phúc 
	if (phucControl == true) {
		switch (key)
		{

		case 'm':
			sophia += 5;
			glutPostRedisplay();
			break;
		case 'M':
			sophia -= 5;
			glutPostRedisplay();
			break;
		case 'k':
			xsf -= 0.2;
			glutPostRedisplay();
			break;
		case 'K':
			xsf += 0.2;
			glutPostRedisplay();
			break;
		case 'l':
			zsf -= 0.2;
			glutPostRedisplay();
			break;
		case 'L':
			zsf += 0.2;
			glutPostRedisplay();
			break;
		case 'x':
			x1 += 0.2;
			break;

		case 'X':
			x1 -= 0.2;
			break;
		case 'z':
			z1 += 0.2;
			break;

		case 'Z':
			z1 -= 0.2;

			break;
		case '1':
			xb += 0.2;
			break;

		case '2':
			xb -= 0.2;
			break;
		case '3':
			zb += 0.2;
			break;

		case '4':
			zb -= 0.2;
			break;
		case '5':
			znk -= 0.2;
			if (znk <= -2.5) znk = -1;
			break;
		case '6':
			if (znk > 0) znk = -0;
			znk += 0.2;
			break;
		case '7':
			if (xg1 >= 1) xg1 = 1;
			xg1 += 0.2;
			break;
		case '8':
			if (xg1 <= -0.5) xg1 = 0.5;
			xg1 -= 0.2;
			break;
		case '9':
			xg2 += 0.2;
			if (xg2 > 1 ) xg2 = 1;
			break;

		case '0':
			xg2 -= 0.2;
			if (xg2 <-1 ) xg2 = -0.5;
			break;
		}
	}
	// Đạt 
	if (datControl == true) {
		switch (key)
		{
		case 'k'://quay cánh
			aaa += 10;
			if (aaa > 100) aaa = 100;
			glutPostRedisplay();
			break;
		case 'K':
			aaa -= 10;
			if (aaa <= 0) aaa = 0;
			glutPostRedisplay();
			break;
		case ';'://di chuyển ngan keo
			NKTuGiong -= 1.5;
			if (NKTuGiong < -2.25) NKTuGiong = -2.25;
			glutPostRedisplay();
			break;
		case ':':
			NKTuGiong += 1.5;
			if (NKTuGiong > 0) NKTuGiong = 0;
			glutPostRedisplay();
			break;
		case '*':
			quay_tu_giuong += 10;
			glutPostRedisplay();
			break;
		case '1':
			xxx += 1;
			glutPostRedisplay();
			break;
		case '2':
			xxx -= 1;
			glutPostRedisplay();
			break;
		case '3':
			zzz += 1;
			glutPostRedisplay();
			break;
		case '4':
			zzz -= 1;
			glutPostRedisplay();
			break;
			// tủ quần áo
		case '.'://quay cánh
			anpha += 10;
			if (anpha > 100) anpha = 100;
			glutPostRedisplay();
			break;
		case '>':
			anpha -= 10;
			if (anpha <= 0) anpha = 0;
			glutPostRedisplay();
			break;
		case 'm':
			beta += 10;
			if (beta > 100) beta = 100;
			glutPostRedisplay();
			break;
		case 'M':
			beta -= 10;
			if (beta <= 0) beta = 0;
			glutPostRedisplay();
			break;
		case '5':
			quay_tu_qao += 10;
			glutPostRedisplay();
			break;
		case '6':
			xxxx += 1;
			glutPostRedisplay();
			break;
		case '7':
			xxxx -= 1;
			glutPostRedisplay();
			break;
		case '8':
			zzzz += 1;
			glutPostRedisplay();
			break;
		case '9':
			zzzz -= 1;
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}
	// Hồng Anh 
	if (honganhControl == true) {
		switch (key)
		{
		case '1'://di chuyển đệm
			di_chuyen_demZ += 0.15, di_chuyen_demY = 0;
			if (di_chuyen_demZ > 0) di_chuyen_demZ = 0;
			if (di_chuyen_demZ > -1.2) xoay_demX = 0;
			if (di_chuyen_demZ > -0.6) xoay_demY = 0;
			glutPostRedisplay();
			break;
		case '2':
			di_chuyen_demZ -= 0.15;
			if (di_chuyen_demZ < -2.4) di_chuyen_demZ = -0.8, di_chuyen_demY = -0.81, xoay_demX = 0;
			glutPostRedisplay();
			break;
		case '3':
			xoay_demY += 5;
			glutPostRedisplay();
			break;
		case '4':
			xoay_demY -= 5;
			glutPostRedisplay();
			break;
		case 'O'://di chuyển gối
			goi1 += 0.15;
			if (goi1 > 0) goi1 = 0;
			glutPostRedisplay();
			break;
		case 'o':
			goi1 -= 0.15;
			if (goi1 < -0.5) goi1 = -1.5;
			glutPostRedisplay();
			break;
		case ')'://di chuyển gối
			goi2 += 0.15;
			if (goi2 > 0.15) goi2 = 0;
			glutPostRedisplay();
			break;
		case '0':
			goi2 -= 0.15;
			if (goi2 < -1.5) goi2 = -1.5;
			glutPostRedisplay();
			break;
		case '5'://di chuyển giường dọc
			xx -= 0.1;
			glutPostRedisplay();
			break;
		case '6':
			xx += 0.1;
			glutPostRedisplay();
			break;
		case '7'://di chuyển giường ngang
			zz += 0.1;
			glutPostRedisplay();
			break;
		case '8':
			zz -= 0.1;
			glutPostRedisplay();
			break;
		case '@'://quay Giường
			aa -= 10;
			glutPostRedisplay();
			break;
		case '#':
			aa += 10;
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}
	mat_dinhhuong = RotateY(matXY[1]) * RotateX(matXY[0]) * vec4(0, 0, 1, 1);
	mat_dinhhuong.w = 1;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	// main function: program starts here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(880, 640);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("Drawing a Cube");

	glutIdleFunc(idle);
	glutTimerFunc(100, timer, 0);
	glewInit();
	
	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(MouseClick);
	glutMainLoop();
	return 0;
}
