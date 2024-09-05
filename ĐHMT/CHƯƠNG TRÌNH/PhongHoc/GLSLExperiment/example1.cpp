//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/

typedef vec4 point4;
typedef vec4 color4;
using namespace std;
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
// Số các đỉnh của các tam giác
const int NumPointsCube = 36;//hinh lap phuong
const int NumPointsTG = 24; //hinh thanh phan
const int NumPointsChop = 18;
// hinh tru mỗi mặt đáy  8 đỉnh =>8 mặt hình chũ nhật, 16 mặt tam giác
// 8*6 + 16*3 = 96
const int NumPointsTru = 96; // hinh tru

point4 points[NumPointsCube + NumPointsTru + NumPointsTG + NumPointsChop]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPointsCube + NumPointsTru + NumPointsTG + NumPointsChop]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPointsCube + NumPointsTru + NumPointsTG + NumPointsChop]; /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/

point4 dinhCube[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 colorCube[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

point4 dinhTG[6];
color4 colorTG[6];

point4 dinhChop[5];
color4 colorChop[5];

point4 dinhTru[18];// 2 dinh o tam
color4 colorTru[10];



#pragma endregion Các biến chức năng
#pragma region 
int on = 1;
GLfloat light_color[] = { 1.0, 1.0, 1.0 };
GLfloat light[3] = { 74, 73, 67 };
color4 light_ambient = color4(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse = color4(1.0, 1.0, 1.0, 1.0);
color4 light_specular = color4(1.0, 1.0, 1.0, 1.0);
point4 light_position = point4(10.0, 10.0, 0, 1.0);
color4 material_ambient(0.2, 0.2, 0.2, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1.0, 1.0, 1.0, 1.0);


float material_shininess = 100;
#pragma endregion Các biến chiếu sáng

vec4 eye(0, 30, -100, 1), at(0, 0, 0, 1), up(0, 1, 0, 1);
GLfloat l = -0.01, r = 0.01, b = -0.01, t = 0.01, zN = 0.02, zF = 50;
mat4 view_mat;
mat4 move_mat;
GLfloat matXY[2]; // doc ngang
vec4 mat_dinhhuong(0, 0, 1, 1);
GLfloat speedTranslate = 0.3;
GLfloat speedRotate = 5;
GLfloat mauBD = 255, mauAnhSang = 1;
#pragma endregion Các biến điều chỉnh eye


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;
mat4 model, view, projection;
GLuint model_loc, view_loc, projection_loc;
GLfloat xx = 0, yy = 0, zz = 0, a = 0, zn = 0;





void initCube() {
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	dinhCube[0] = point4(-0.5, -0.5, 0.5, 1.0);
	dinhCube[1] = point4(-0.5, 0.5, 0.5, 1.0);
	dinhCube[2] = point4(0.5, 0.5, 0.5, 1.0);
	dinhCube[3] = point4(0.5, -0.5, 0.5, 1.0);
	dinhCube[4] = point4(-0.5, -0.5, -0.5, 1.0);
	dinhCube[5] = point4(-0.5, 0.5, -0.5, 1.0);
	dinhCube[6] = point4(0.5, 0.5, -0.5, 1.0);
	dinhCube[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	colorCube[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	colorCube[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	colorCube[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	colorCube[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	colorCube[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	colorCube[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	colorCube[6] = color4(1.0, 0.5, 0.0, 1.0); // orange
	colorCube[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}

void initTG() {
	dinhTG[0] = point4(-0.5, -0.5, 0.5, 1.0);
	dinhTG[1] = point4(0.5, -0.5, 0.5, 1.0);
	dinhTG[2] = point4(0.5, 0.5, 0.5, 1.0);
	dinhTG[3] = point4(-0.5, -0.5, -0.5, 1.0);
	dinhTG[4] = point4(0.5, -0.5, -0.5, 1.0);
	dinhTG[5] = point4(0.5, 0.5, -0.5, 1.0);

	colorTG[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	colorTG[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	colorTG[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	colorTG[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	colorTG[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	colorTG[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
}

void initChop() {
	dinhChop[0] = point4(0.0, 0.5, 0.0, 1.0);
	dinhChop[1] = point4(-0.5, -0.5, -0.5, 1.0);
	dinhChop[2] = point4(-0.5, -0.5, 0.5, 1.0);
	dinhChop[3] = point4(0.5, -0.5, 0.5, 1.0);
	dinhChop[4] = point4(0.5, -0.5, -0.5, 1.0);

	colorChop[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	colorChop[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	colorChop[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	colorChop[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	colorChop[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
}

void initTru() {
	float tren = 0.5;
	float duoi = -0.5;
	dinhTru[0] = point4(0, tren, 0, 1);//tam tren
	for (int i = 1; i <= 8; i++) {
		float banKinh = 0.5;
		float pi = (i - 1) * 45.0 / 360 * 2 * M_PI;
		// Tính x va z
		float x = banKinh * cosf(pi);
		float z = banKinh * sinf(pi);
		dinhTru[i * 2 - 1] = point4(x, tren, z, 1);
		dinhTru[i * 2] = point4(x, duoi, z, 1);
	}
	dinhTru[17] = point4(0, duoi, 0, 1);//tam duoi

	colorTru[0] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	colorTru[1] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	colorTru[2] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	colorTru[3] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	colorTru[4] = color4(1.0, 0.0, 1.0, 1.0); // tím
	colorTru[5] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	colorTru[6] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	colorTru[7] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	colorTru[8] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	colorTru[9] = color4(1.0, 0.0, 1.0, 1.0); // tím
}
int Index = 0;
void quadCube(int a, int b, int c, int d) { /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
	vec4 u = dinhCube[b] - dinhCube[a];
	vec4 v = dinhCube[c] - dinhCube[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[a]; Index++;
	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[b]; Index++;
	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[c]; Index++;
	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[a]; Index++;
	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[c]; Index++;
	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[d]; Index++;
}

void quadTG(int a, int b, int c) {
	vec4 u = dinhCube[b] - dinhCube[a];
	vec4 v = dinhCube[c] - dinhCube[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[a]; Index++;
	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[b]; Index++;
	normals[Index] = normal; colors[Index] = colorCube[a]; points[Index] = dinhCube[c]; Index++;
}

void quadChop(int a, int b, int c) {
	vec4 u = dinhChop[b] - dinhChop[a];
	vec4 v = dinhChop[c] - dinhChop[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = colorChop[a]; points[Index] = dinhChop[a]; Index++;
	normals[Index] = normal; colors[Index] = colorChop[a]; points[Index] = dinhChop[b]; Index++;
	normals[Index] = normal; colors[Index] = colorChop[a]; points[Index] = dinhChop[c]; Index++;
}
// taoj mot mat hinh chu nhat cua hinh tru
void quadTruHinhchunhat(int a, int b, int c, int d) {
	vec4 u = dinhTru[b] - dinhTru[a];
	vec4 v = dinhTru[c] - dinhTru[b];
	vec3 phapTuyen = normalize(cross(u, v));

	// a -> b -> c -> a -> c -> d
	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[a]; Index++;
	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[b]; Index++;
	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[c]; Index++;
	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[a]; Index++;
	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[c]; Index++;
	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[d]; Index++;
}

void quadTruHinhtamgiac(int a, int b, int c) {
	vec4 u = dinhTru[b] - dinhTru[a];
	vec4 v = dinhTru[c] - dinhTru[b];
	vec3 phapTuyen = normalize(cross(u, v));

	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[a]; Index++;
	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[b]; Index++;
	normals[Index] = phapTuyen; colors[Index] = colorTru[a]; points[Index] = dinhTru[c]; Index++;
}

void makeColorCube(void) { /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
	quadCube(1, 0, 3, 2);
	quadCube(2, 3, 7, 6);
	quadCube(3, 0, 4, 7);
	quadCube(6, 5, 1, 2);
	quadCube(4, 5, 6, 7);
	quadCube(5, 4, 0, 1);
}

void makeColorTG(void) {
	quadTG(0, 1, 2);
	quadTG(1, 4, 5);
	quadTG(1, 5, 2);
	quadTG(2, 5, 3);
	quadTG(2, 3, 0);
	quadTG(3, 5, 4);
	quadTG(4, 1, 0);
	quadTG(4, 0, 3);
}

void makecolorChop(void) {
	quadChop(0, 1, 2);
	quadChop(0, 2, 3);
	quadChop(0, 3, 4);
	quadChop(0, 4, 1);
	quadChop(1, 4, 3);
	quadChop(1, 3, 2);
}

void makeColorTru(void) {
	//cac mat ben
	quadTruHinhchunhat(2, 1, 3, 4);
	quadTruHinhchunhat(4, 3, 5, 6);
	quadTruHinhchunhat(6, 5, 7, 8);
	quadTruHinhchunhat(8, 7, 9, 10);
	quadTruHinhchunhat(10, 9, 11, 12);
	quadTruHinhchunhat(12, 11, 13, 14);
	quadTruHinhchunhat(14, 13, 15, 16);
	quadTruHinhchunhat(16, 15, 1, 2);
	//mat duoi
	quadTruHinhtamgiac(2, 17, 16);
	quadTruHinhtamgiac(16, 17, 14);
	quadTruHinhtamgiac(14, 17, 12);
	quadTruHinhtamgiac(12, 17, 10);
	quadTruHinhtamgiac(10, 17, 8);
	quadTruHinhtamgiac(8, 17, 6);
	quadTruHinhtamgiac(6, 17, 4);
	quadTruHinhtamgiac(4, 17, 2);

	// mat tren
	quadTruHinhtamgiac(1, 0, 3);
	quadTruHinhtamgiac(3, 0, 5);
	quadTruHinhtamgiac(5, 0, 7);
	quadTruHinhtamgiac(7, 0, 9);
	quadTruHinhtamgiac(9, 0, 11);
	quadTruHinhtamgiac(11, 0, 13);
	quadTruHinhtamgiac(13, 0, 15);
	quadTruHinhtamgiac(15, 0, 1);


}

void generateGeometry(void) {
	initCube();	makeColorCube();
	initTG(); makeColorTG();
	initChop();	makecolorChop();
	initTru(); makeColorTru();
}

void vehinhlapphuong(mat4 kichthuoc, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * kichthuoc);
	glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);    /*Vẽ các tam giác*/
}

void vehinhTG(mat4 kichthuoc, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * kichthuoc);
	glDrawArrays(GL_TRIANGLES, 36, NumPointsTG);    /*Vẽ các tam giác*/
}

void vehinhChop(mat4 kichthuoc, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * kichthuoc);
	glDrawArrays(GL_TRIANGLES, 60, NumPointsChop);    /*Vẽ các tam giác*/
}

void vehinhTru(mat4 kichthuoc, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * kichthuoc);
	glDrawArrays(GL_TRIANGLES, 78, NumPointsTru);// 36 + 24 + 18 
}


void initGPUBuffers(void) {
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

void shaderSetup(void) {
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

	model_loc = glGetUniformLocation(program, "Model_View");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	//glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
	glClearColor(0.53, 0.8, 0.92, 1.0);   //thiet lap mau xanh cho nen
}

float TinhMau(int a) {
	return (float)a / 255;
}

color4 DoiMau(int a, int b, int c) {
	return color4(TinhMau(a), TinhMau(b), TinhMau(c), 1.0);
}


void ToMau(vec4 color) {
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
	light_ambient = DoiMau(light[0], light[1], light[2]);
	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}


#pragma endregion Dựng mô hình

//========================== NỀN ĐẤT ===============================
void nenDat(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = DoiMau(195, 123, 97);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(100, 0.05, 100), Translate(x, y, z));
}

void khungCanh()
{
	material_diffuse = DoiMau(157, 75, 7);
	ToMau(material_diffuse);
	vehinhChop(Scale(15, 15, 15), Translate(-35.8, 8, 23.3));
	// draw building
	material_diffuse = DoiMau(36, 175, 218);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(1.69, 63, 19.4), Translate(-9.3, 31.8, -25.126));
	material_diffuse = DoiMau(113, 85, 215);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(1.69, 63, 19.4), Translate(9.024, 31.8, -25.126));
	material_diffuse = DoiMau(59, 225, 134);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(19.6, 63, 1.27), Translate(0, 31.8, -34.174));
	material_diffuse = DoiMau(15, 116, 127);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(19.6, 63, 1.27), Translate(0, 31.8, -16));
	material_diffuse = DoiMau(231, 153, 210);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(18.3, 3.7, 17.8), Translate(-0.3, 56.413, -25.7));
}

//===================== MÔ HÌNH CẦN CẨU ============================
mat4 model_maycau; // sử dụng 1 model để xử lý các chuyển động chung của máy cẩu
mat4 model_topmaycau; // sử dụng 1 model để sử lý phần trên của máy cẩu
void baseTop(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = DoiMau(219, 147, 31);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(5.5, 0.3, 4.2), model_maycau * Translate(x, y, z));
}

void boxControl(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = DoiMau(148, 136, 183);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(2.77, 1, 4.2), model_maycau * Translate(x, y, z));
}

void areaDriver() {
	material_diffuse = DoiMau(223, 219, 57);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(2.7, 1.38, 0.25), model_maycau * Translate(1.4, 0.43, 2));
	vehinhlapphuong(Scale(2.7, 1.38, 0.25), model_maycau * Translate(1.4, 0.43, -2));
	vehinhlapphuong(Scale(0.25, 1.38, 2.18), model_maycau * Translate(2.63, 0.43, -1));
	vehinhlapphuong(Scale(0.25, 1.38, 4.2), model_maycau * Translate(0.15, 0.43, 0));
	vehinhlapphuong(Scale(2.8, 0.3, 4.2), model_maycau * Translate(1.4, 3.2, 0));
	material_diffuse = DoiMau(219, 147, 31);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_maycau * Translate(0.17, 1.5, 1.98));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_maycau * Translate(2.62, 1.5, 1.98));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_maycau * Translate(0.17, 1.5, -1.98));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_maycau * Translate(2.62, 1.5, -1.98));

	//draw chair
	material_diffuse = DoiMau(163, 141, 231);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(1.15, 0.65, 1.15), model_maycau * Translate(1.4, 0.2, 0));
	material_diffuse = DoiMau(123, 148, 231);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(1.15, 1.11, 0.3), model_maycau * Translate(1.4, 1, -0.45));

	// draw truc quay
	material_diffuse = DoiMau(148, 136, 183);
	ToMau(material_diffuse);
	vehinhTru(Scale(2, 1, 2), model_maycau * Translate(0, -0.8, 0));
}

void window() {
	material_diffuse = DoiMau(100, 186, 113);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.25, 1.73, 1.85), model_maycau * Translate(2.63, 0.6, 1));
	vehinhlapphuong(Scale(0.25, 1.73, 0.36), model_maycau * Translate(2.63, 2.22, 1.74));
	vehinhlapphuong(Scale(0.25, 1.73, 0.36), model_maycau * Translate(2.63, 2.22, 0.27));
	vehinhlapphuong(Scale(0.25, 0.28, 1.85), model_maycau * Translate(2.63, 2.9, 1));
	material_diffuse = DoiMau(100, 100, 193);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.15, 1.73, 1.85), model_maycau * Translate(2.63, 2, 1));
}

void base(GLfloat x, GLfloat y, GLfloat z, GLfloat goc) {
	material_diffuse = DoiMau(209, 205, 33);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(1.3, 3.77, 1.2), model_maycau * Translate(x, y, z) * RotateX(goc));
}

void base3(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = DoiMau(209, 205, 33);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(1.3, 3.6, 1.11), model_maycau * Translate(x, y, z) * RotateX(-25));
}

void dauCau()
{
	material_diffuse = DoiMau(209, 205, 33);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(2, 0.35, 1.9), model_maycau * Translate(-1.4, 1, 6.3));
	vehinhlapphuong(Scale(2, 1.62, 0.35), model_maycau * Translate(-1.4, 0.33, 7.2) * RotateX(180));
	vehinhTG(Scale(1.9, 0.22, 1.8), model_maycau * Translate(-0.5, 0, 6.3) * RotateZ(-90));
	vehinhTG(Scale(1.9, 0.22, 1.8), model_maycau * Translate(-2.3, 0, 6.3) * RotateZ(-90));
}


GLfloat meta[] = { 0, 0, 0, 0 };
void armMachine()
{
	// base 1
	model_maycau = model_maycau * Translate(-1.4, 0.5, 1.1) * RotateX(meta[0]) * Translate(1.4, -0.5, -1.1);
	base(-1.4, 2.3, 1.1, 20);
	// base 2
	model_maycau = model_maycau * Translate(-1.37, 3.9, 2.5) * RotateX(meta[1]) * Translate(1.37, -3.9, -2.5);
	base(-1.37, 3.9, 3.11, 83);
	// base 3
	model_maycau = model_maycau * Translate(-1.37, 4.5, 5.17) * RotateX(meta[2]) * Translate(1.37, -4.5, -5.17);
	base3(-1.37, 2.85, 5.17);
	// dau cau
	model_maycau = model_maycau * Translate(-1.4, 1, 5.6) * RotateX(meta[3]) * Translate(1.4, -1, -5.6);
	dauCau();
}
GLfloat gocquaycua; // Khai báo góc quay của bánh xe => thuận tiện cho việc thêm các chức năng quay cho bánh xe

void topFull()
{
	model_topmaycau = model_maycau;
	baseTop(0, -0.2, 0);
	boxControl(-1.4, 0.45, 0);
	areaDriver();
	model_maycau = model_topmaycau * Translate(2.6, 0, 1.9) * RotateY(gocquaycua) * Translate(-2.6, 0, -1.9);
	window();
	model_maycau = model_topmaycau;
	armMachine();
}

// chuyen sang phan duoi may cau

GLfloat gocquaybx; // Khai báo góc quay của bánh xe => thuận tiện cho việc thêm các chức năng quay cho bánh xe
void banhXe(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = DoiMau(50, 50, 50);
	ToMau(material_diffuse);
	vehinhTru(Scale(1.37, 0.51, 1.37), model_maycau * Translate(x, y, z) * RotateX(gocquaybx) * RotateZ(90));
}

void banhTrai()
{
	material_diffuse = DoiMau(92, 92, 92);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.5, 0.11, 4.3), model_maycau * Translate(2.5, -0.87, 0.05));
	vehinhlapphuong(Scale(0.5, 0.11, 4.3), model_maycau * Translate(2.5, -2.13, 0.05));
	vehinhlapphuong(Scale(0.05, 0.5, 5), model_maycau * Translate(2.78, -1.5, 0));
	banhXe(2.5, -1.5, 2.1);
	banhXe(2.5, -1.5, 0.75);
	banhXe(2.5, -1.5, -0.65);
	banhXe(2.5, -1.5, -2);
}

void banhPhai()
{
	material_diffuse = DoiMau(92, 92, 92);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.5, 0.11, 4.3), model_maycau * Translate(-2.5, -0.87, 0.05));
	vehinhlapphuong(Scale(0.5, 0.11, 4.3), model_maycau * Translate(-2.5, -2.13, 0.05));
	vehinhlapphuong(Scale(0.05, 0.5, 5), model_maycau * Translate(-2.78, -1.5, 0));
	banhXe(-2.5, -1.5, 2.1);
	banhXe(-2.5, -1.5, 0.75);
	banhXe(-2.5, -1.5, -0.65);
	banhXe(-2.5, -1.5, -2);
}

void bottomFull()
{
	// Base
	vehinhlapphuong(Scale(4.6, 0.5, 4.2), model_maycau * Translate(0, -1.5, 0));
	// Banh xe
	// => banh trai
	banhTrai();
	// => banh phai
	banhPhai();
}

GLfloat controlmc[] = { 0, 0, 0 };
GLfloat gocquayfulmc = 0, gocquaytopmc = 0;
vec4 test(0, 0, 0);
void mayCauFull(GLfloat x, GLfloat y, GLfloat z)
{
	model_maycau = Translate(x, y, z) * Translate(test) * RotateY(gocquayfulmc);
	bottomFull();
	model_maycau = model_maycau * RotateY(gocquaytopmc);
	topFull();
	
}



//===================== MÔ HÌNH MÁY XÚC ============================
mat4 model_mayxuc; // sử dụng 1 model để xử lý các chuyển động chung của máy xúc
GLfloat gocquaycuamx, gocquaycan;
void khungmx() {
	material_diffuse = DoiMau(231, 175, 71);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.75, 1.53, 7.75), model_mayxuc * Translate(0, -1.7, -1.6));
	vehinhlapphuong(Scale(0.7, 1.4, 0.7), model_mayxuc * Translate(-1.5, -1.7, -2));
	vehinhlapphuong(Scale(3.4, 0.6, 0.7), model_mayxuc * Translate(0, -2, -2));
	vehinhlapphuong(Scale(0.7, 1.4, 0.7), model_mayxuc * Translate(1.5, -1.7, -2));
	material_diffuse = DoiMau(201, 155, 71);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(3.6, 1, 7.5), model_mayxuc * Translate(0, -0.65, -1.7));
	vehinhlapphuong(Scale(3, 3, 3), model_mayxuc * Translate(0, 1, -3.7));
	material_diffuse = DoiMau(95, 95, 95);
	ToMau(material_diffuse);
	vehinhTru(Scale(0.5, 4.8, 0.5), model_mayxuc * Translate(0, -2, 1.5) * RotateZ(90));
	vehinhTru(Scale(0.5, 4.8, 0.5), model_mayxuc * Translate(0, -2, -4.5) * RotateZ(90));
	vehinhTru(Scale(0.5, 1.3, 0.5), model_mayxuc * Translate(-1, 2.5, -4.5));

	material_diffuse = DoiMau(166, 163, 26);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.25, 1.38, 4.2), model_mayxuc * Translate(-1.3, 0.43, 0));
	vehinhlapphuong(Scale(0.25, 1.38, 2), model_mayxuc * Translate(1.2, 0.5, 1));
	vehinhlapphuong(Scale(2.7, 1.38, 0.25), model_mayxuc * Translate(0, 0.43, 2));
	vehinhlapphuong(Scale(2.7, 1.38, 0.25), model_mayxuc * Translate(0, 0.43, -2));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_mayxuc * Translate(-1.25, 1.5, 2));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_mayxuc * Translate(1.25, 1.5, 2));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_mayxuc * Translate(-1.25, 1.5, -2));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_mayxuc * Translate(1.25, 1.5, -2));
	vehinhlapphuong(Scale(3.0, 0.3, 4.2), model_mayxuc * Translate(0, 3.2, 0));

	material_diffuse = DoiMau(0, 112, 192);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(1.15, 1.11, 0.3), model_mayxuc * Translate(0, 1, -0.45));
	vehinhlapphuong(Scale(1.15, 0.65, 1.15), model_mayxuc * Translate(0, 0.2, 0));
}

void windowmx() {
	material_diffuse = DoiMau(100, 186, 113);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.25, 1.73, 1.85), model_mayxuc * Translate(1.2, 0.6, -0.9));
	vehinhlapphuong(Scale(0.25, 1.73, 0.36), model_mayxuc * Translate(1.2, 2.22, -0.185));
	vehinhlapphuong(Scale(0.25, 1.73, 0.36), model_mayxuc * Translate(1.2, 2.22, -1.65));
	vehinhlapphuong(Scale(0.25, 0.28, 1.85), model_mayxuc * Translate(1.2, 2.9, -1));
	material_diffuse = DoiMau(100, 100, 193);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.15, 1.73, 1.85), model_mayxuc * Translate(1.2, 2, -0.9));
}

void canXuc()
{
	material_diffuse = DoiMau(142, 139, 22);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.4, 1.17, 4.2), model_mayxuc * Translate(1.5, -0.4, 3) * RotateX(30));
	vehinhlapphuong(Scale(0.5, 1.17, 2.25), model_mayxuc * Translate(1.5, -1.11, 5) * RotateX(-18));
	vehinhlapphuong(Scale(0.4, 1.17, 4.2), model_mayxuc * Translate(-1.5, -0.4, 3) * RotateX(30));
	vehinhlapphuong(Scale(0.5, 1.17, 2.25), model_mayxuc * Translate(-1.5, -1.11, 5) * RotateX(-18));
	vehinhlapphuong(Scale(6.6, 3.38, 0.25), model_mayxuc * Translate(0, -1.2, 6.2) * RotateX(-21.626));
	vehinhlapphuong(Scale(6.8, 0.4, 1.83), model_mayxuc * Translate(0, 0.75, 6.3) * RotateX(-30.5));
	vehinhlapphuong(Scale(6.8, 0.3, 1.6), model_mayxuc * Translate(0, -2.55, 7.5));
	vehinhlapphuong(Scale(0.42, 1.65, 2.25), model_mayxuc * Translate(3.33, -0.02, 6.8) * RotateX(60));
	vehinhlapphuong(Scale(0.42, 1.65, 2.25), model_mayxuc * Translate(3.33, -1.5, 7.33) * RotateX(88));
	vehinhlapphuong(Scale(0.42, 1.65, 2.25), model_mayxuc * Translate(-3.33, -0.02, 6.8) * RotateX(60));
	vehinhlapphuong(Scale(0.42, 1.65, 2.25), model_mayxuc * Translate(-3.33, -1.5, 7.33) * RotateX(88));
	// răng cưa
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(-3.25, -2.5, 8.5));
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(-2.55, -2.5, 8.5));
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(-1.75, -2.5, 8.5));
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(-0.9, -2.5, 8.5));
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(0, -2.5, 8.5));
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(3.25, -2.5, 8.5));
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(2.55, -2.5, 8.5));
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(1.75, -2.5, 8.5));
	vehinhlapphuong(Scale(0.35, 0.15, 1), model_mayxuc * Translate(0.9, -2.5, 8.5));
}

GLfloat gocquaybanhxemx;

void banhxe1mx(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = DoiMau(50, 50, 50);
	ToMau(material_diffuse);
	vehinhTru(Scale(4, 1.35, 4), model_mayxuc * Translate(x, y, z) * RotateX(gocquaybanhxemx) * RotateZ(90));
}

void banhxemx()
{
	banhxe1mx(2.7, -2, 1.5);
	banhxe1mx(2.7, -2, -4.5);
	banhxe1mx(-2.7, -2, 1.5);
	banhxe1mx(-2.7, -2, -4.5);
}

vec4 controlmx(0, 0, 0);
GLfloat gocquayfulmx;
mat4 model_phu;

void mayXucFull(GLfloat x, GLfloat y, GLfloat z)
{
	model_mayxuc = Translate(x, y, z) * Translate(controlmx) * RotateY(gocquayfulmx);
	model_phu = model_mayxuc;
	khungmx();
	model_mayxuc = model_phu * Translate(1.2, 2.22, -1.83) * RotateY(gocquaycuamx) * Translate(-1.2, -2.22, 1.83);
	windowmx();
	model_mayxuc = model_phu * Translate(1.5, -0.4, 1.2) * RotateX(gocquaycan) *Translate(-1.5, 0.4, -1.2);
	canXuc();
	model_mayxuc = model_phu;
	banhxemx();
}


//===================== MÔ HÌNH XE LU ============================
mat4 model_xelu; // sử dụng 1 model để xử lý các chuyển động chung của xe lu
GLfloat gocquaycuaxl;
void khungxl() {
	material_diffuse = DoiMau(180, 136, 55);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.75, 1.53, 4.94), model_xelu * Translate(0, -1.7, -2.63));
	vehinhlapphuong(Scale(3.6, 1, 5.2), model_xelu * Translate(0, -0.65, -2.7));
	material_diffuse = DoiMau(202, 198, 32);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.25, 1.38, 4.2), model_xelu * Translate(-1.3, 0.43, -3.089));
	vehinhlapphuong(Scale(0.25, 1.38, 2), model_xelu * Translate(1.2, 0.5, -4.15));
	vehinhlapphuong(Scale(2.7, 1.38, 0.25), model_xelu * Translate(0, 0.43, -5.09));
	vehinhlapphuong(Scale(2.7, 1.38, 0.25), model_xelu * Translate(0, 0.43, -1.09));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_xelu * Translate(-1.25, 1.5, -1.11));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_xelu * Translate(1.25, 1.5, -1.11));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_xelu * Translate(-1.25, 1.5, -5.08));
	vehinhlapphuong(Scale(0.25, 3.3, 0.25), model_xelu * Translate(1.25, 1.5, -5.08));
	vehinhlapphuong(Scale(2.8, 0.3, 4.2), model_xelu * Translate(0, 3.2, -3.09));
	vehinhlapphuong(Scale(0.5, 1.17, 8.8), model_xelu * Translate(2, -0.9, 2.25));
	vehinhlapphuong(Scale(0.5, 1.17, 8.8), model_xelu * Translate(-2, -0.9, 2.25));
	vehinhlapphuong(Scale(4, 1.38, 0.25), model_xelu * Translate(0, -0.9, 6.5));

	material_diffuse = DoiMau(74, 109, 222);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(1.1, 0.5, 0.8), model_xelu * Translate(0, 0, -2.75));
	vehinhlapphuong(Scale(1.15, 1.11, 0.3), model_xelu * Translate(0, 0.4, -3.3));
	material_diffuse = DoiMau(70, 70, 70);
	ToMau(material_diffuse);
	vehinhTru(Scale(0.5, 4.8, 0.5), model_xelu * Translate(0, -2, -4.5) * RotateZ(90));
}

GLfloat gocquaybanhxelu, gocquaycuaxelu;
void banhxelu()
{
	material_diffuse = DoiMau(50, 50, 50);
	ToMau(material_diffuse);
	vehinhTru(Scale(4, 1.35, 4), model_xelu * Translate(2.7, -2, -4.5) * RotateX(gocquaybanhxelu) * RotateZ(90));
	vehinhTru(Scale(4, 1.35, 4), model_xelu * Translate(-2.7, -2, -4.5) * RotateX(gocquaybanhxelu) * RotateZ(90));
	vehinhTru(Scale(6, 3.36, 6), model_xelu * Translate(0, -1, 3.11) * RotateX(gocquaybanhxelu) * RotateZ(90));
}

void windowxelu() {
	material_diffuse = DoiMau(100, 186, 113);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.25, 1.73, 1.85), model_xelu * Translate(1.2, 0.6, -2.2));
	vehinhlapphuong(Scale(0.25, 1.73, 0.36), model_xelu * Translate(1.2, 2.22, -1.45));
	vehinhlapphuong(Scale(0.25, 1.73, 0.36), model_xelu * Translate(1.2, 2.22, -3));
	vehinhlapphuong(Scale(0.25, 0.28, 1.85), model_xelu * Translate(1.2, 2.9, -2.2));
	material_diffuse = DoiMau(100, 100, 193);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.15, 1.73, 1.85), model_xelu * Translate(1.2, 2, -2.2));
}

vec4 dichuyenxelu(0, 0, 0);
GLfloat gocquayfulxl;
void xeLuFull(GLfloat x, GLfloat y, GLfloat z)
{
	model_xelu = Translate(x, y, z) * Translate(dichuyenxelu) * RotateY(gocquayfulxl);
	model_phu = model_xelu;
	khungxl();
	model_xelu = model_phu * Translate(1.2, 2.22, -1.4) * RotateY(gocquaycuaxelu) * Translate(-1.2, -2.22, 1.4);
	windowxelu();
	model_xelu = model_phu;
	banhxelu();
}


//===================== MÔ HÌNH XE TẢI ============================
mat4 model_xetai; // sử dụng 1 model để xử lý các chuyển động chung của xe tải
GLfloat gocquaycuatraixetai, gocquaycuaphaixetai;
void khungxt() {
	material_diffuse = DoiMau(180, 136, 55);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(3.93, 2.93, 22.1), model_xetai * Translate(0, -2.3, -0.58));
	vehinhlapphuong(Scale(9.54, 1, 18), model_xetai * Translate(0, -0.65, -3.5));

	material_diffuse = DoiMau(47, 85, 151);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(9.55, 0.625, 7.15), model_xetai * Translate(0, -0.82, 8.6));
	vehinhlapphuong(Scale(9.6, 2.84, 0.825), model_xetai * Translate(0, 0.25, 11.8));
	vehinhlapphuong(Scale(9.6, 0.5, 6.67), model_xetai * Translate(0, 6, 8.75));
	vehinhlapphuong(Scale(0.65, 7.33, 0.98), model_xetai * Translate(4.5, 2.6, 11.65));
	vehinhlapphuong(Scale(0.65, 7.33, 0.98), model_xetai * Translate(-4.5, 2.6, 11.65));
	vehinhlapphuong(Scale(9.6, 7.33, 0.825), model_xetai * Translate(0, 2.6, 5.5));
	material_diffuse = DoiMau(92, 92, 92);
	ToMau(material_diffuse);
	vehinhTru(Scale(0.5, 10.6, 0.5), model_xetai * Translate(0, -3.3, 8.95) * RotateZ(90));
	vehinhTru(Scale(0.5, 10.6, 0.5), model_xetai * Translate(0, -3.3, -0.22) * RotateZ(90));
	vehinhTru(Scale(0.5, 10.6, 0.5), model_xetai * Translate(0, -3.3, -4.5) * RotateZ(90));
	vehinhTru(Scale(0.5, 10.6, 0.5), model_xetai * Translate(0, -3.3, -8.7) * RotateZ(90));
	material_diffuse = DoiMau(138, 162, 31);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(3, 3, 1.45), model_xetai * Translate(-2.5, 0.86, 6.5));
	vehinhlapphuong(Scale(3, 1.2, 2.7), model_xetai * Translate(-2.45, -0.06, 8.5));
	vehinhlapphuong(Scale(3, 3, 1.45), model_xetai * Translate(2, 0.86, 6.5));
	vehinhlapphuong(Scale(3, 1.2, 2.7), model_xetai * Translate(2, -0.06, 8.5));
	material_diffuse = DoiMau(106, 231, 226);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(8.8, 4.3, 0.35), model_xetai * Translate(0, 3.6, 11.8));
}
GLfloat gocquaybanhxetai;
void banhxe1xt(GLfloat x, GLfloat y, GLfloat z)
{
	material_diffuse = DoiMau(50, 50, 50);
	ToMau(material_diffuse);
	vehinhTru(Scale(4, 1.35, 4), model_xetai * Translate(x, y, z) * RotateX(gocquaybanhxetai) * RotateZ(90));
}

void windowleftxt()
{
	material_diffuse = DoiMau(85, 85, 231);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.712, 2.68, 5.25), model_xetai * Translate(4.4, 0.76, 8.5));
	material_diffuse = DoiMau(106, 231, 226);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.467, 3.95, 5.23), model_xetai * Translate(4.4, 3.82, 8.5));
}

void windowrightxt()
{
	material_diffuse = DoiMau(85, 85, 231);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.712, 2.68, 5.25), model_xetai * Translate(-4.4, 0.76, 8.5));
	material_diffuse = DoiMau(106, 231, 226);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.467, 3.95, 5.23), model_xetai * Translate(-4.4, 3.82, 8.5));
}

void banhtraixt()
{
	banhxe1xt(5.98, -3.3, -8.68);
	banhxe1xt(5.98, -3.3, -4.5);
	banhxe1xt(5.98, -3.3, -0.22);
	banhxe1xt(5.98, -3.3, 8.95);

}

void banhphaixt()
{
	banhxe1xt(-5.98, -3.3, -8.68);
	banhxe1xt(-5.98, -3.3, -4.5);
	banhxe1xt(-5.98, -3.3, -0.22);
	banhxe1xt(-5.98, -3.3, 8.95);
}

GLfloat gocquaykhungsau;

void khungsau()
{
	material_diffuse = DoiMau(105, 88, 192);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(9.54, 0.46, 18), model_xetai * Translate(0, 0.1, -3.5));
	vehinhlapphuong(Scale(0.8, 3.46, 17), model_xetai * Translate(-4.4, 1.83, -3.56));
	vehinhlapphuong(Scale(0.8, 3.46, 17), model_xetai * Translate(4.4, 1.83, -3.56));
	vehinhlapphuong(Scale(9.7, 3.46, 0.832), model_xetai * Translate(0, 1.92, 4.6));
	vehinhlapphuong(Scale(9.7, 3.46, 0.832), model_xetai * Translate(0, 1.92, -11.85));
}

GLfloat gocquayfulxt;
vec4 dichuyenxetai(0, 0, 0);
void xeTaiFull(GLfloat x, GLfloat y, GLfloat z)
{
	model_xetai = Translate(x, y, z) * Translate(dichuyenxetai) * RotateY(gocquayfulxt);
	model_phu = model_xetai;
	khungxt();
	banhtraixt();
	banhphaixt();
	model_xetai = model_phu * Translate(0, 0.1, -11.5) * RotateX(gocquaykhungsau) * Translate(0, -0.1, 11.5);
	khungsau();
	model_xetai = model_phu * Translate(4.4, 0.76, 11.125) * RotateY(gocquaycuatraixetai) * Translate(-4.4, -0.76, -11.125);
	windowleftxt();
	model_xetai = model_phu * Translate(-4.4, 0.76, 11.125) * RotateY(gocquaycuaphaixetai) * Translate(4.4, -0.76, -11.125);
	windowrightxt();
}

//===================== MÔ HÌNH CẦN TRỤC THÁP ============================
mat4 model_tructhap; // sử dụng 1 model để xử lý các chuyển động chung của cần trục tháp

void khungtruc() {
	material_diffuse = DoiMau(75, 84, 148);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(4.37, 26.7, 4.25), model_tructhap * Translate(-0.3, 71.3, -24.94));
	vehinhlapphuong(Scale(4.37, 3.06, 13.4), model_tructhap * Translate(-0.3, 72.8, -30.5));
}

void dayTruc()
{
	material_diffuse = DoiMau(50, 50, 50);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.8, 30, 0.8), model_tructhap * Translate(-0.3, 58.67, 7.58));
	material_diffuse = DoiMau(231, 175, 71);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(13.3, 4.7, 14.5), model_tructhap * Translate(-0.3, 44.2, 7));
}

void trucxoay()
{
	material_diffuse = DoiMau(75, 84, 148);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(4.37, 3.06, 36.1), model_tructhap * Translate(-0.3, 72.8, -8.95));
	material_diffuse = DoiMau(50, 50, 50);
	ToMau(material_diffuse);
	vehinhlapphuong(Scale(0.8, 30, 0.8), model_tructhap * Translate(-0.3, 58.67, 7.58));
}

GLfloat gocquaytructhap, thaday;

void trucThapFul()
{
	model_tructhap = Translate(0, 0, 0);
	khungtruc();
	model_tructhap = Translate(-0.3, 72.8, -27) * RotateY(gocquaytructhap) * Translate(0.3, -72.8, 27);
	trucxoay();
	model_tructhap = model_tructhap * Translate(0, thaday, 0);
	dayTruc();
}
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

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );      
	model = RotateY(90);
	Eye();
	nenDat(0, 0, 0);
	mayCauFull(0, 2.2, 0);
	mayXucFull(10, 4, 0);
	xeLuFull(-10, 4, 0);
	xeTaiFull(20, 5, 0);
	khungCanh();
	trucThapFul();
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

void nangkhung(int a)
{
	gocquaykhungsau -= 5;
	if (gocquaykhungsau < -45)
	{
		gocquaykhungsau = -45;
		return;
	}
	glutPostRedisplay();
	glutTimerFunc(400, nangkhung, a);
}

void hakhung(int a)
{
	gocquaykhungsau += 5;
	if (gocquaykhungsau > 0)
	{
		gocquaykhungsau = 0;
		return;
	}
	glutPostRedisplay();
	glutTimerFunc(400, hakhung, a);
}

void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

    switch ( key ) {
		// Chỉnh tọa độ của mắt
	case 'w': //Đi cam tiến
		eye = eye + RotateY(matXY[1]) * vec4(0.0, 0.0, speedTranslate, 1);
		break;
	case 's': //Đi cam lùi
		eye = eye - RotateY(matXY[1]) * vec4(0.0, 0.0, speedTranslate, 1);
		break;
	case 'd'://Đi cam sang phải
		eye = eye - RotateY(matXY[1]) * vec4(speedTranslate, 0.0, 0.0, 1);
		break;
	case 'a': //Đi cam sang trái
		eye = eye + RotateY(matXY[1]) * vec4(speedTranslate, 0.0, 0.0, 1);
		break;
	case 'q': //Đẩy cam xuống dưới
		eye = eye - RotateY(matXY[1]) * vec4(0.0, speedTranslate, 0.0, 1);
		break;
	case 'e'://Đẩy cam lên trên
		eye = eye + RotateY(matXY[1]) * vec4(0.0, speedTranslate, 0.0, 1);
		break;
	case 'l': //Quay cam phải
		matXY[1] -= speedRotate;
		if (matXY[1] >= 360) matXY[1] -= 360;
		if (matXY[1] < 0) matXY[1] += 360;
		break;
	case 'j'://Quay cam trái
		matXY[1] += speedRotate;
		if (matXY[1] >= 360) matXY[1] -= 360;
		if (matXY[1] < 0) matXY[1] += 360;
		break;
	case 'i'://Quay cam lên
		matXY[0] -= speedRotate;
		if (matXY[0] < -75) {
			matXY[0] = -75;
		}
		break;
	case 'k'://Quay cam xuống
		matXY[0] += speedRotate;
		if (matXY[0] > 75) {
			matXY[0] = 75;
		}
		break;
	case '/':
		light_color[0] = light_color[1] = light_color[2] = 0.2;
		glutPostRedisplay();
		break;
	case '?':
		light_color[0] = light_color[1] = light_color[2] = 1.0;
		glutPostRedisplay();
		break;
	// DIEU KHIEN CAN CAU
	// xu ly Cua
	case '=':
		gocquaycua -= 5;
		if (gocquaycua < -95)
			gocquaycua = -95;
		glutPostRedisplay();
		break;
	case '-':
		gocquaycua += 5;
		if (gocquaycua > 0)
			gocquaycua = 0;
		glutPostRedisplay();
		break;
	// xu ly tay can cau
		// => base 1
	case 'z':
		meta[0] -= 5;
		if (meta[0] < -30)
			meta[0] = -30;
		glutPostRedisplay();
		break;
	case 'Z':
		meta[0] += 5;
		if (meta[0] > 30)
			meta[0] = 30;
		glutPostRedisplay();
		break;
		// => base 2
	case 'x':
		meta[1] -= 5;
		if (meta[1] < -5)
			meta[1] = -5;
		glutPostRedisplay();
		break;
	case 'X':
		meta[1] += 5;
		if (meta[1] > 30)
			meta[1] = 30;
		glutPostRedisplay();
		break;
		// => base 3
	case 'c':
		meta[2] -= 5;
		if (meta[2] < -30)
			meta[2] = -30;
		glutPostRedisplay();
		break;
	case 'C':
		meta[2] += 5;
		if (meta[2] > 10)
			meta[2] = 10;
		glutPostRedisplay();
		break;
		// => dau cau
	case 'v':
		meta[3] -= 5;
		if (meta[3] < -10)
			meta[3] = -10;
		glutPostRedisplay();
		break;
	case 'V':
		meta[3] += 5;
		if (meta[3] > 90)
			meta[3] = 90;
		glutPostRedisplay();
		break;
	// xu ly phan tren may cau
	case 'b':
		gocquaytopmc -= 5;
		glutPostRedisplay();
		break;
	case 'B':
		gocquaytopmc += 5;
		glutPostRedisplay();
		break;

	// xu ly di chuyen xe cau
	case 'm':
		gocquayfulmc += 5;
		if (gocquayfulmc > 360)
			gocquayfulmc = -360;
		glutPostRedisplay();
		break;
	case 'M':
		gocquayfulmc -= 5;
		if (gocquayfulmc < -360)
			gocquayfulmc = 360;
		glutPostRedisplay();
		break;
		// => di tien
	case 'n':
		test = test + RotateY(gocquayfulmc) * vec4(0.0, 0.0, speedTranslate, 1);
		gocquaybx += 15;
		glutPostRedisplay();
		break;
		// => di lui
	case 'N':
		test = test - RotateY(gocquayfulmc) * vec4(0.0, 0.0, speedTranslate, 1);
		gocquaybx -= 15;
		glutPostRedisplay();
		break;

	// DIEU KHIEN MÁY XÚC
		// xu ly Cửa
	case '[':
		gocquaycuamx += 5;
		if (gocquaycuamx > 95)
			gocquaycuamx = 95;
		glutPostRedisplay();
		break;
	case ']':
		gocquaycuamx -= 5;
		if (gocquaycuamx < 0)
			gocquaycuamx = 0;
		glutPostRedisplay();
		break;
		// => xu ly Cần xúc
	case ',':
		gocquaycan -= 5;
		if (gocquaycan < -45)
			gocquaycan = -45;
		glutPostRedisplay();
		break;
	case '.':
		gocquaycan += 5;
		if (gocquaycan > 0)
			gocquaycan = 0;
		glutPostRedisplay();
		break;
		// xu ly di chuyen may xuc
	case 'g':
		gocquayfulmx += 5;
		if (gocquayfulmx > 360)
			gocquayfulmx = -360;
		glutPostRedisplay();
		break;
	case 'G':
		gocquayfulmx -= 5;
		if (gocquayfulmx < -360)
			gocquayfulmx = 360;
		glutPostRedisplay();
		break;
		// => di tien
	case 'h':
		controlmx = controlmx + RotateY(gocquayfulmx) * vec4(0.0, 0.0, speedTranslate, 1);
		gocquaybanhxemx += 15;
		glutPostRedisplay();
		break;
		// => di lui
	case 'H':
		controlmx = controlmx - RotateY(gocquayfulmx) * vec4(0.0, 0.0, speedTranslate, 1);
		gocquaybanhxemx -= 15;
		glutPostRedisplay();
		break;


	// DIEU KHIEN XE LU
		// xu ly Cửa
	case '1':
		gocquaycuaxelu -= 5;
		if (gocquaycuaxelu < -95)
			gocquaycuaxelu = -95;
		glutPostRedisplay();
		break;
	case '!':
		gocquaycuaxelu += 5;
		if (gocquaycuaxelu > 0)
			gocquaycuaxelu = 0;
		glutPostRedisplay();
		break;
		// xu ly di chuyen xe lu
	case '2':
		gocquayfulxl += 5;
		if (gocquayfulxl > 360)
			gocquayfulxl = -360;
		glutPostRedisplay();
		break;
	case '@':
		gocquayfulxl -= 5;
		if (gocquayfulxl < -360)
			gocquayfulxl = 360;
		glutPostRedisplay();
		break;
		// => di tien
	case '3':
		dichuyenxelu = dichuyenxelu + RotateY(gocquayfulxl) * vec4(0.0, 0.0, speedTranslate, 1);
		gocquaybanhxelu += 15;
		glutPostRedisplay();
		break;
		// => di lui
	case '#':
		dichuyenxelu = dichuyenxelu - RotateY(gocquayfulxl) * vec4(0.0, 0.0, speedTranslate, 1);
		gocquaybanhxelu -= 15;
		glutPostRedisplay();
		break;
	// DIEU KHIEN XE TẢI
		// xu ly khung sau
	case '4':
		glutTimerFunc(400, nangkhung, 400);	
		break;
	case '$':
		glutTimerFunc(400, hakhung, 400);
		break;
		// xu ly quay cua trai
	case '5':
		gocquaycuatraixetai -= 5;
		if (gocquaycuatraixetai < -95)
			gocquaycuatraixetai = -95;
		glutPostRedisplay();
		break;
	case '%':
		gocquaycuatraixetai += 5;
		if (gocquaycuatraixetai > 0)
			gocquaycuatraixetai = 0;
		glutPostRedisplay();
		break;
		// xu ly quay cua phai
	case '6':
		gocquaycuaphaixetai += 5;
		if (gocquaycuaphaixetai > 95)
			gocquaycuaphaixetai = 95;
		glutPostRedisplay();
		break;
	case '^':
		gocquaycuaphaixetai -= 5;
		if (gocquaycuaphaixetai < 0)
			gocquaycuaphaixetai = 0;
		glutPostRedisplay();
		break;

		// xu ly di chuyen xe tai
	case '7':
		gocquayfulxt += 5;
		if (gocquayfulxt > 360)
			gocquayfulxt = -360;
		glutPostRedisplay();
		break;
	case '&':
		gocquayfulxt -= 5;
		if (gocquayfulxt < -360)
			gocquayfulxt = 360;
		glutPostRedisplay();
		break;
		// => di tien
	case '8':
		dichuyenxetai = dichuyenxetai + RotateY(gocquayfulxt) * vec4(0.0, 0.0, speedTranslate, 1);
		gocquaybanhxetai += 15;
		glutPostRedisplay();
		break;
		// => di lui
	case '*':
		dichuyenxetai = dichuyenxetai - RotateY(gocquayfulxt) * vec4(0.0, 0.0, speedTranslate, 1);
		gocquaybanhxetai -= 15;
		glutPostRedisplay();
		break;

	// DIEU KHIEN TRỤC THÁP
		// xu ly quay khung 
	case '9':
		gocquaytructhap -= 5;
		if (gocquaycuatraixetai < -360)
			gocquaycuatraixetai = 360;
		glutPostRedisplay();
		break;
	case '(':
		gocquaytructhap += 5;
		if (gocquaycuatraixetai > 360)
			gocquaycuatraixetai = -360;
		glutPostRedisplay();
		break;
		// xu ly tha day
	case '0':
		thaday -= 1;
		if (thaday < -30)
			thaday = -30;
		glutPostRedisplay();
		break;
	case ')':
		thaday += 1;
		if (thaday > 0)
			thaday = 0;
		glutPostRedisplay();
		break;
    case 033:			// 033 is Escape key octal value
        exit(1);		// quit program
        break;
    }
	
	mat_dinhhuong = RotateY(matXY[1]) * RotateX(matXY[0]) * vec4(0, 0, 1, 1);
	mat_dinhhuong.w = 1;


	glutPostRedisplay();
}


int main( int argc, char **argv )
{
	// main function: program starts here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 640, 640 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Drawing a construction" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
    glutKeyboardFunc( keyboard );                  
	glutMouseFunc(MouseClick);
	glutMainLoop();
    return 0;
}
