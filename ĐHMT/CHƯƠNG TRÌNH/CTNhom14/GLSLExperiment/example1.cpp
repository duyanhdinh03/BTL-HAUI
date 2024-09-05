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
GLuint model_loc;
GLuint view_loc;
GLuint projection_loc;

mat4 instance;
vec4 colorCode;
mat4 model;
GLfloat theta[3] = { 0, 0, 0 };
mat4 model_view;


mat4 mdl;
mat4 projection;
mat4 view;
mat4 ctm;


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


	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}

float xeye = 0, yeye = 4, zeye = 4;
float xat = 0, yat = 2, zat = -3.5;
float xup = 0, yup = 1, zup = 0;

void projection_view() {
	point4 eye(xeye, yeye, zeye, 1);
	point4 at(xat, yat, zat, 1);
	vec4 up(xup, yup, zup, 1);
	mat4 view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	mat4 projection = Frustum(-0.5, 0.5, -0.5, 0.5, 0.5, 20);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
}

mat4 TDuong;

void Cube(mat4 mt)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * TDuong * mt);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
//void setColor(color4 color) {
//	material_diffuse = color;
//	diffuse_product = light_diffuse * material_diffuse;
//	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
//}

/// Dũng start
GLfloat convertDecimal(int x) {
	return x / 255.0f;
}
color4 convertRGB(int r, int g, int b) {
	return color4(convertDecimal(r), convertDecimal(g), convertDecimal(b), 1.0);
}

int bd = 1;
bool on = true;
GLint light[3] = {0, 0, 0};
void setColor(color4 color) {
	if(bd == 0) {
		color4 light_ambient(0.05, 0.05, 0.05, 1.0);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 light_specular(1.0, 1.0, 1.0, 1.0);

		color4 material_ambient(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0.5, 0.4, 0.0, 1.0);
		color4 material_specular(1.0, 0.8, 0.0, 1.0);
		float material_shininess = 100.0;

		light[0] = 30;
		light[1] = 30;
		light[2] = 30;
		material_diffuse = color;

		light_ambient = convertRGB(light[0], light[1], light[2]);
		light_diffuse = convertRGB(light[0], light[1], light[2]);
		light_specular = convertRGB(light[0], light[1], light[2]);

		ambient_product = light_ambient * material_ambient;
		diffuse_product = light_diffuse * material_diffuse;
		specular_product = light_specular * material_specular;

		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
		glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
		glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	}
	else {
		color4 light_ambient(0.2, 0.2, 0.2, 1.0);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 light_specular(1.0, 1.0, 1.0, 1.0);

		color4 material_ambient(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
		color4 material_specular(1.0, 0.8, 0.0, 1.0);
		float material_shininess = 100.0;
		
		material_diffuse = color;

		ambient_product = light_ambient * material_ambient;
		diffuse_product = light_diffuse * material_diffuse;
		specular_product = light_specular * material_specular;

		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
		glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
		glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	}

}
//Mã màu
//-----Mã màu nền
vec4 MauNen = vec4(0.525, 0.367, 0.165, 1.0);// Màu nền có màu nâu nhạt
vec4 MauTuong = vec4(0.701, 0.760, 0.721, 1.0);//Màu tường có màu xám nhạt 
vec4 MauCuaSau = vec4(0.205, 0.018, 0.030, 1.0);//Màu cửa sau có màu đỏ đậm 
vec4 MauCuaSo = vec4(0.000, 0.990, 0.962, 0.5);// Màu cửa sổ có màu xanh dương sáng với độ trong suốt là 0.5.
vec4 MauKhungCuaSo = vec4(0.205, 0.018, 0.030, 1.0);//Màu khung cửa sổ có màu đỏ đậm
vec4 MauBanAn = vec4(0.918, 1.0, 0.167, 1.0);// Màu bàn ăn có màu vàng nhạt
vec4 MauGheAn = vec4(0.003, 0.211, 0.912, 1.0);//Màu ghế ăn có màu xanh dương
vec4 MauTuBep = vec4(0.255, 0.078, 0.052, 1.0);//Màu tủ bếp có màu nâu đậm
vec4 MauKhungTuLanh = vec4(0.640, 0.733, 0.912, 1.0);//Màu khung tủ lạnh có màu xanh dương nhạt
vec4 MauCachTuNganDong = vec4(0.755, 0.657, 0.445, 1.0);//Màu cách tủ ngăn đông có màu nâu nhạt
vec4 MauCachTuNganLanh = vec4(0.835, 0.727, 0.492, 1.0);//Màu cách tủ ngăn lạnh có màu nâu nhạt


float wNB = 3, dNB = 2.65, hNB = 3.5; // chiều d r c của mô hình nhà
float doDatT = 0.15; // độ dày của tường trái 3.9
float caoCuaSo = 0.6, daiCuaSo = 1.2, dayCuaSo = 0.075, keoCuaSo1 = 0, keoCuaSo2 = 0, checkKeo = false;
float dayKhungCuaSo = 0.05;
float daiTBPhai = (dNB - daiCuaSo) / 2, caoTBPhai = (hNB - caoCuaSo) * 3 / 4;
void cuaSoBep() {
	setColor(MauKhungCuaSo);
	//vị trí khung cửa trên tường phải
	model = Translate(wNB + doDatT, caoTBPhai, 0);
	//cạnh trái của khung cửa sổ
	instance = model * Translate(dayKhungCuaSo * 0.5, caoCuaSo * 0.5, -dayKhungCuaSo * 0.5 - daiTBPhai) * Scale(dayKhungCuaSo, caoCuaSo, dayKhungCuaSo);
	Cube(instance);
	//cạnh phải của khung cửa sổ
	instance = model * Translate(dayKhungCuaSo * 0.5, caoCuaSo * 0.5, -dayKhungCuaSo * 0.5 - daiTBPhai - daiCuaSo + dayKhungCuaSo) * Scale(dayKhungCuaSo, caoCuaSo, dayKhungCuaSo);
	Cube(instance);
	//cạnh dưới của khung cửa sổ
	instance = model * Translate(dayKhungCuaSo * 0.5, dayKhungCuaSo * 0.5, -daiCuaSo * 0.5 - daiTBPhai) * Scale(dayKhungCuaSo, dayKhungCuaSo, daiCuaSo);
	Cube(instance);
	//cạnh trên của khung cửa sổ
	instance = model * Translate(dayKhungCuaSo * 0.5, dayKhungCuaSo * 0.5 + caoCuaSo - dayKhungCuaSo, -daiCuaSo * 0.5 - daiTBPhai) * Scale(dayKhungCuaSo, dayKhungCuaSo, daiCuaSo);
	Cube(instance);
	// cánh cửa
	setColor(MauCuaSo);
	instance = model * Translate(dayCuaSo * 0.5, (caoCuaSo - dayKhungCuaSo * 2) * 0.5 + dayKhungCuaSo, -(daiCuaSo - dayKhungCuaSo * 2) / 2 * 0.5 - daiTBPhai - dayKhungCuaSo - keoCuaSo1)
		* Scale(dayCuaSo, caoCuaSo - dayKhungCuaSo * 2, (daiCuaSo - dayKhungCuaSo * 2) / 2);
	Cube(instance);
	instance = model * Translate(dayCuaSo * 0.5 + dayCuaSo, (caoCuaSo - dayKhungCuaSo * 2) * 0.5 + dayKhungCuaSo, -(daiCuaSo - dayKhungCuaSo * 2) / 2 * 0.5 - daiTBPhai - dayKhungCuaSo - (daiCuaSo - dayKhungCuaSo * 2) / 2 + keoCuaSo2)
		* Scale(dayCuaSo, caoCuaSo - dayKhungCuaSo * 2, (daiCuaSo - dayKhungCuaSo * 2) / 2);
	Cube(instance);
}
void tuongBepBenPhai() {
	// đường viền tường trước bên phải -- start
	setColor(MauTuong);
	model = Translate(wNB + doDatT, 0, 0);
	instance = model * Translate(doDatT * 0.5, hNB * 0.5, -daiTBPhai * 0.5) * Scale(doDatT, hNB, daiTBPhai);
	Cube(instance);
	instance = model * Translate(doDatT * 0.5, hNB * 0.5, -daiTBPhai * 0.5 - daiCuaSo - daiTBPhai) * Scale(doDatT, hNB, daiTBPhai);
	Cube(instance);
	instance = model * Translate(doDatT * 0.5, caoTBPhai * 0.5, -daiCuaSo * 0.5 - daiTBPhai) * Scale(doDatT, caoTBPhai, daiCuaSo);
	Cube(instance);
	instance = model * Translate(doDatT * 0.5, (hNB - caoTBPhai - caoCuaSo) * 0.5 + caoCuaSo + caoTBPhai, -daiCuaSo * 0.5 - daiTBPhai) * Scale(doDatT, hNB - caoTBPhai - caoCuaSo, daiCuaSo);
	Cube(instance);
	// đường viền tường trước bên phải -- end
}

float rongCuaSau = 1, dayCuaSau = 0.05, caoCuaSau = 2, moCuaBepSau = 0, checkMoCuaBepSau = false;
float rongtuongBepBenTrai = 0.3;
float rongtuongBepBenPhai = wNB + doDatT * 2 - rongCuaSau - rongtuongBepBenTrai;
GLfloat  rTNCS = 0.05, daiTNCS = 0.2;

void TNCS(mat4 mt,int k) {
	instance = Translate(daiTNCS * 0.5,0, k*(0.5 * rTNCS + daiTNCS - dayCuaSau - rTNCS))*RotateZ(moCuaBepSau*90/100)*Translate(-daiTNCS*0.5,0,0)* Scale(daiTNCS, rTNCS, rTNCS);
	Cube(mt * instance);
	instance = Translate(0.5* rTNCS + daiTNCS*0.5, 0, k*(0.5 * daiTNCS - rTNCS)) * RotateZ(moCuaBepSau * 90 / 100)  * Scale(rTNCS, rTNCS, daiTNCS);
	Cube(mt * instance);
}
void CuaSauBep() {
	setColor(MauCuaSau);
	model = Translate(rongtuongBepBenTrai, 0, -dNB) * RotateY(-moCuaBepSau);
	instance = model * Translate(rongCuaSau * 0.5, caoCuaSau * 0.5, -dayCuaSau * 0.5) * Scale(rongCuaSau, caoCuaSau, dayCuaSau);
	Cube(instance);  //Cửa sau bếp
	setColor(vec4(MauTuong));
	// núm cửa sau trong
	instance = model * Translate(rongCuaSau - daiTNCS + rTNCS*0.5, caoCuaSau * 0.5,0 );
	TNCS(instance,1);
	// núm cửa sau ngoài
	instance = model * Translate(rongCuaSau - daiTNCS + rTNCS * 0.5, caoCuaSau * 0.5, -rTNCS);
	TNCS(instance,-1);

}
void tuongBepBepSau() {
	setColor(MauTuong);
	model = Translate(0, 0, -dNB);
	instance = model * Translate(rongtuongBepBenTrai * 0.5, hNB * 0.5, -doDatT * 0.5) * Scale(rongtuongBepBenTrai, hNB, doDatT);
	Cube(instance);  // tường sau bếp trái
	instance = model * Translate(rongtuongBepBenPhai * 0.5 + rongtuongBepBenTrai + rongCuaSau, hNB * 0.5, -doDatT * 0.5)
		* Scale(rongtuongBepBenPhai, hNB, doDatT);
	Cube(instance);// tường sau bếp phải
	instance = model * Translate(rongCuaSau * 0.5 + rongtuongBepBenTrai, (hNB - caoCuaSau) * 0.5 + caoCuaSau, -doDatT * 0.5)
		* Scale(rongCuaSau, hNB - caoCuaSau, doDatT);
	Cube(instance); // tường trên cửa sau
}
void TuongBenTraiBep() {
	setColor(MauTuong);
	//đường viền tường trước bên trái
	instance = Translate(doDatT * 0.5, hNB * 0.5, -dNB * 0.5) * Scale(doDatT, hNB, dNB);
	Cube(instance);
}
void MatNenBep() {
	setColor(MauNen);
	// mặt nền 
	instance = Translate((wNB + doDatT * 2) * 0.5, -doDatT * 0.5, -(dNB + doDatT) * 0.5) * Scale(wNB + doDatT * 2, doDatT, dNB + doDatT);
	Cube(instance);
}
void KhungBep() {
	MatNenBep();
	//TuongBenTraiBep();
	tuongBepBenPhai();
	cuaSoBep();
	tuongBepBepSau();
	CuaSauBep();
}
float caoTuLanh = 1.4, rongTulanh = 0.5, dayTuLanh = 0.3, dayManhTuLanh = 0.01;
float moTuNganDong = 0, moTuNganLanh = 0, checkMoTuLanh = false;
void TL(mat4 modelTuLanh) {
	setColor(MauKhungTuLanh);
	model = modelTuLanh;
	instance = model * Translate(-rongTulanh * 0.5, caoTuLanh * 0.5, dayManhTuLanh * 0.5) * Scale(rongTulanh, caoTuLanh, dayManhTuLanh);
	Cube(instance);  // mảnh tủ lạnh trong
	instance = model * Translate(-dayManhTuLanh * 0.5, caoTuLanh * 0.5, dayTuLanh * 0.5) * Scale(dayManhTuLanh, caoTuLanh, dayTuLanh);
	Cube(instance);  // mảnh tủ lạnh bên phải
	instance = model * Translate(-dayManhTuLanh * 0.5 - rongTulanh + dayManhTuLanh, caoTuLanh * 0.5, dayTuLanh * 0.5) * Scale(dayManhTuLanh, caoTuLanh, dayTuLanh);
	Cube(instance);  // mảnh tủ lạnh bên trái
	instance = model * Translate(-rongTulanh * 0.5, dayManhTuLanh * 0.5, dayTuLanh * 0.5) * Scale(rongTulanh, dayManhTuLanh, dayTuLanh);
	Cube(instance);  // mảnh tủ lạnh dưới
	instance = model * Translate(-rongTulanh * 0.5, dayManhTuLanh * 0.5 + caoTuLanh - dayManhTuLanh, dayTuLanh * 0.5) * Scale(rongTulanh, dayManhTuLanh, dayTuLanh);
	Cube(instance);  // mảnh tủ lạnh trên
	instance = model * Translate(-rongTulanh * 0.5, dayManhTuLanh * 0.5 + caoTuLanh * 2 / 3, dayTuLanh * 0.5) * Scale(rongTulanh, dayManhTuLanh, dayTuLanh);
	Cube(instance);  // mảnh tủ lạnh giữa

	setColor(MauCachTuNganDong);
	//model
	model = model * Translate(0, 0, dayTuLanh - dayManhTuLanh);
	// cánh tủ ngăn đông
	instance = model * RotateY(moTuNganDong) * Translate(-rongTulanh * 0.5, caoTuLanh * 1 / 3 * 0.5 + caoTuLanh * 2 / 3, dayManhTuLanh * 0.5)
		* Scale(rongTulanh, caoTuLanh * 1 / 3, dayManhTuLanh);
	Cube(instance);
	// cánh tủ ngăn mát
	setColor(MauCachTuNganLanh);
	instance = model * RotateY(moTuNganLanh) * Translate(-rongTulanh * 0.5, caoTuLanh * 2 / 3 * 0.5, dayManhTuLanh * 0.5)
		* Scale(rongTulanh, caoTuLanh * 2 / 3, dayManhTuLanh);
	Cube(instance);
	model = model * Translate(0, 0, dayTuLanh - dayManhTuLanh);
}
void TuLanh() {
	model = Translate(rongtuongBepBenTrai + rongCuaSau + rongTulanh + 0.1, 0, -dNB);
	TL(model);
}
float whMatGhe = 0.3, dayGhe1 = 0.02, keoGheNgoi = 0, checkKeoGheNgoi = false;
float caoChanGhe = 0.5, wChanGhe = 0.02;
void GheAn(mat4 vt) {
	setColor(MauGheAn);
	// mặt ghế
	instance = vt * Translate(0, caoChanGhe + dayGhe1 * 0.5, 0) * Scale(whMatGhe, dayGhe1, whMatGhe);
	Cube(instance);
	// đỡ lưng
	instance = vt * Translate(0, whMatGhe * 1.2 * 0.5 + caoChanGhe, +dayGhe1 * 0.5 - whMatGhe * 0.5) * Scale(whMatGhe, whMatGhe * 1.2, dayGhe1);
	Cube(instance);
	// chân 1
	instance = vt * Translate(wChanGhe - whMatGhe / 2, caoChanGhe * 0.5, wChanGhe - whMatGhe / 2) * Scale(wChanGhe, caoChanGhe, wChanGhe);
	Cube(instance);
	// chân 2
	instance = vt * Translate(wChanGhe - whMatGhe / 2, caoChanGhe * 0.5, -wChanGhe + whMatGhe / 2) * Scale(wChanGhe, caoChanGhe, wChanGhe);
	Cube(instance);
	// chân 3
	instance = vt * Translate(-wChanGhe + whMatGhe / 2, caoChanGhe * 0.5, wChanGhe - whMatGhe / 2) * Scale(wChanGhe, caoChanGhe, wChanGhe);
	Cube(instance);
	// chân 4
	instance = vt * Translate(-wChanGhe + whMatGhe / 2, caoChanGhe * 0.5, -wChanGhe + whMatGhe / 2) * Scale(wChanGhe, caoChanGhe, wChanGhe);
	Cube(instance);

}
void initKhoiTru(mat4 vt, float r, float h) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, instance);
	glutSolidCylinder(r, h, 64, 64);

}

float bkDia = 0.05, dayDiaAn = 0.01;
void DiaAn(mat4 vt) {

	// đĩa ăn
	instance = model_view * TDuong * vt * Translate(0, 0.5 * bkDia, 0) * RotateX(90);
	initKhoiTru(instance, bkDia, dayDiaAn);
}
float banKinhBanAn = 0.5, doDayBanAn = 0.02;
float chieuCaoBanAn = 0.6;
float quayBanAn = 0;
mat4 modelBanAn;
void banAn() {
	model = Translate(2, 0, -1.2);
	setColor(vec4(0.480, 0.245, 0.000, 1.0));
	modelBanAn = model * RotateY(quayBanAn) * Translate(0, chieuCaoBanAn, 0);
	// mặt bàn ăn1
	setColor(vec4(0.755, 0.475, 0.148, 1.0));
	for (int i = 1; i < 360; i++)
	{
		instance = modelBanAn * RotateY(i) * Translate(-banKinhBanAn * 0.5, doDayBanAn * 0.5, banKinhBanAn * 0.5)
			* Scale(banKinhBanAn, doDayBanAn, banKinhBanAn);
		Cube(instance);
	}
	// mặt bàn ăn2
	for (int i = 1; i < 360; i++)
	{
		instance = modelBanAn * RotateY(i) * Translate(-banKinhBanAn / 2 * 0.5, doDayBanAn * 0.5 + doDayBanAn, banKinhBanAn / 2 * 0.5)
			* Scale(banKinhBanAn / 2, doDayBanAn, banKinhBanAn / 2);
		Cube(instance);
	}
	// trụ bàn ăn
	for (int i = 1; i < 360; i++)
	{
		instance = model * RotateY(i) * Translate(-doDayBanAn * 0.5, chieuCaoBanAn * 0.5, doDayBanAn * 0.5)
			* Scale(doDayBanAn, chieuCaoBanAn, doDayBanAn);
		Cube(instance);
	}
	// chân bàn ăn
	for (int i = 1; i < 360; i++)
	{
		instance = model * RotateY(i) * Translate(-banKinhBanAn / 4 * 0.5, doDayBanAn * 5 * 0.5, banKinhBanAn / 5 * 0.5)
			* Scale(banKinhBanAn / 4, doDayBanAn * 5, banKinhBanAn / 5);
		Cube(instance);
	}
	// ghế thứ nhất
	instance = model * Translate(0, 0, -banKinhBanAn * 1.2 - keoGheNgoi);
	GheAn(instance);
	// ghế thứ hai
	instance = model * Translate(0, 0, banKinhBanAn * 1.2 + keoGheNgoi) * RotateY(180);
	GheAn(instance);
	// ghế thứ ba
	instance = model * Translate(-banKinhBanAn * 1.2 - keoGheNgoi, 0, 0) * RotateY(90);
	GheAn(instance);
	// ghế thứ tư
	instance = model * Translate(banKinhBanAn * 1.2 + keoGheNgoi, 0, 0) * RotateY(-90);
	GheAn(instance);

	//đĩa ăn 1
	instance = modelBanAn * Translate(banKinhBanAn, doDayBanAn, 0);
	DiaAn(instance);
	//đĩa ăn 2
	instance = modelBanAn * Translate(-banKinhBanAn, doDayBanAn, 0);
	DiaAn(instance);
	//đĩa ăn 3
	instance = modelBanAn * Translate(0, doDayBanAn, banKinhBanAn);
	DiaAn(instance);
	//đĩa ăn 4
	instance = modelBanAn * Translate(0, doDayBanAn, -banKinhBanAn);
	DiaAn(instance);
}
float caoTuTreo, rongTuTreo, dayTuTreo, dayManhTutreo;
float moTuTreo = 0, checkMoTuTreo = false;
void TuBep(mat4 mt) {
	setColor(vec4(0.395, 0.386, 0.175, 1.0));
	// mặt sau
	instance = mt * Translate(-rongTuTreo * 0.5, caoTuTreo * 0.5, dayManhTutreo * 0.5)
		* Scale(rongTuTreo, caoTuTreo, dayManhTutreo);
	Cube(instance);
	setColor(vec4(0.425, 0.173, 0.044, 1.0));
	// thanh trái
	instance = mt * Translate(-dayManhTutreo * 0.5 - rongTuTreo + dayManhTutreo, caoTuTreo * 0.5, dayTuTreo * 0.5)
		* Scale(dayManhTutreo, caoTuTreo, dayTuTreo);
	Cube(instance);
	// thanh phải
	instance = mt * Translate(-dayManhTutreo * 0.5, caoTuTreo * 0.5, dayTuTreo * 0.5)
		* Scale(dayManhTutreo, caoTuTreo, dayTuTreo);
	Cube(instance);
	// thanh dưới
	instance = mt * Translate(-rongTuTreo * 0.5, dayManhTutreo * 0.5, dayTuTreo * 0.5)
		* Scale(rongTuTreo, dayManhTutreo, dayTuTreo);
	Cube(instance);
	// thanh trên
	instance = mt * Translate(-rongTuTreo * 0.5, dayManhTutreo * 0.5 + caoTuTreo - dayManhTutreo, dayTuTreo * 0.5)
		* Scale(rongTuTreo, dayManhTutreo, dayTuTreo);
	Cube(instance);
	// cánh tủ
	setColor(vec4(0.359, 0.425, 0.194, 1.0));
	mat4 tuTreoModel = RotateY(moTuTreo);
	instance = mt * Translate(0, 0, dayTuTreo - dayManhTutreo) * tuTreoModel * Translate(-(rongTuTreo - dayManhTutreo * 2) * 0.5, (caoTuTreo - dayManhTutreo * 2) * 0.5 + dayManhTutreo, dayManhTutreo * 0.5)
		* Scale(rongTuTreo - dayManhTutreo * 2, caoTuTreo - dayManhTutreo * 2, dayManhTutreo);
	Cube(instance);
	// núm mở tủ
	setColor(vec4(0.975, 0.851, 0.136, 1.0));
	instance = mt * Translate(0, 0, +dayTuTreo) * tuTreoModel * Translate(-(rongTuTreo / 10) * 0.5 - rongTuTreo * 0.7, dayManhTutreo * 0.5 + caoTuTreo * 0.5, dayManhTutreo * 0.5)
		* Scale(rongTuTreo / 10, dayManhTutreo, dayManhTutreo);
	Cube(instance);
}
void tuTreo(float Tx, float Ty, float Tz, int R) {
	mat4 modelTuTreo = Translate(rongTuTreo + rongCuaSau + rongtuongBepBenTrai + rongTulanh + 0.1 + Tx, 0, Tz);
	instance = modelTuTreo * Translate(0, caoCuaSau * 2 / 3 + Ty, -dNB) * RotateY(R) * RotateZ(2 * R);
	TuBep(instance);

}
void DayTuTreo() {
	// tủ trên
	caoTuTreo = 0.4, rongTuTreo = 0.3, dayTuTreo = 0.1, dayManhTutreo = 0.01;
	tuTreo(0, 0, 0, 0);
	tuTreo(rongTuTreo, 0, 0, 0);
	tuTreo(rongTuTreo * 2, 0, 0, 0);
	tuTreo(rongTuTreo * 3, 0, 0, 0);
}
void TuBepNau(float Tx) {
	caoTuTreo = 0.7, rongTuTreo = 0.4, dayTuTreo = 0.3, dayManhTutreo = 0.02;
	instance = Translate(rongTuTreo + rongCuaSau + rongtuongBepBenTrai + rongTulanh + 0.1 + Tx * rongTuTreo, 0, -dNB);
	TuBep(instance);
}

float xzBepGa = 0.2, yBepga = 0.05;
float batBepGa = 0;
bool checkBatBepGa = false;
void bepGa(mat4 mt) {
	// thân bếp
	setColor(vec4(0.685, 0.679, 0.531, 1.0));
	instance = mt * Translate(0, yBepga * 0.5, 0) * Scale(xzBepGa, yBepga, xzBepGa);
	Cube(instance);

	setColor(vec4(0.025, 0.025, 0.019, 1.0));
	// đỡ nồi 1 
	model = Translate(0.05 * 0.5 - xzBepGa / 2, yBepga, 0);
	instance = mt * RotateY(45) * model * Translate(0, 0.01 * 0.5, 0) * Scale(0.05, 0.01, 0.01);
	Cube(instance);
	// đỡ nồi 2 
	instance = mt * RotateY(45 + 90) * model * Translate(0, 0.01 * 0.5, 0) * Scale(0.05, 0.01, 0.01);
	Cube(instance);
	// đỡ nồi 3 
	instance = mt * RotateY(-45) * model * Translate(0, 0.01 * 0.5, 0) * Scale(0.05, 0.01, 0.01);
	Cube(instance);
	// đỡ nồi 4
	instance = mt * RotateY(-45 - 90) * model * Translate(0, 0.01 * 0.5, 0) * Scale(0.05, 0.01, 0.01);
	Cube(instance);

	setColor(vec4(0.675, 0.052, 0.097, 1.0));
	//Đỡ bếp 1 
	model = Translate(0.01 * 0.5 - xzBepGa / 2, 0, 0);
	instance = mt * RotateY(45) * model * Translate(0, -0.02 * 0.5, 0) * Scale(0.01, 0.02, 0.01);
	Cube(instance);
	//Đỡ bếp 2
	instance = mt * RotateY(45 + 90) * model * Translate(0, -0.02 * 0.5, 0) * Scale(0.01, 0.02, 0.01);
	Cube(instance);
	//Đỡ bếp 3
	instance = mt * RotateY(-45) * model * Translate(0, -0.02 * 0.5, 0) * Scale(0.01, 0.02, 0.01);
	Cube(instance);
	//Đỡ bếp 4
	instance = mt * RotateY(-45 - 90) * model * Translate(0, -0.02 * 0.5, 0) * Scale(0.01, 0.02, 0.01);
	Cube(instance);
	// Nút bật bếp
	instance = mt * Translate(0, 0.02 + yBepga / 2, xzBepGa / 2 + 0.01) * RotateZ(batBepGa) * Scale(0.02, 0.03, 0.02);
	Cube(instance);
}
void bepNau() {
	TuBepNau(0);	// tủ bếp dưới 1
	TuBepNau(1);	// tủ bếp dưới 2
	TuBepNau(2);	// tủ bếp dưới 3
	// bình ga 
	float rBinhGa = 0.12, hBinhGa = 0.4;
	instance = model_view * TDuong * Translate(rBinhGa + rongCuaSau + rongtuongBepBenTrai + rongTulanh + 0.2, dayManhTutreo + hBinhGa * 0.5 + 0.2, -dNB + dayManhTutreo + rBinhGa) * RotateX(90);
	initKhoiTru(instance, rBinhGa, hBinhGa);
	//NoiComDien();   // Nồi cơm điện
	bepGa(Translate(xzBepGa + rongCuaSau + rongtuongBepBenTrai + rongTulanh + 0.2, 0.7 + 0.02, -dNB + xzBepGa));
	bepGa(Translate(xzBepGa + rongCuaSau + rongtuongBepBenTrai + rongTulanh + 0.2 + xzBepGa, 0.7 + 0.02, -dNB + xzBepGa));
}

float hCanhQuat = 0.2, dCanhQuat = 0.1, dayCanhQuat = 0.001;
int quayQuat = 0, quayDauQuat = 0;
float nangQuat = 0;
bool checkNangQuat = false,checkQuayDauQuat = false;
mat4 modelThan ,modelDau;

void CanhQuat(int Rz) {
	//Cánh quạt
	setColor(vec4(0.912, 0.378, 0.273, 1.0));
	instance = model * modelThan * modelDau * RotateZ(quayQuat) * RotateZ(Rz) * Translate(0, hCanhQuat * 0.5, dayCanhQuat * 0.5 + 0.075) 
		* Scale(dCanhQuat, hCanhQuat, dayCanhQuat);
	Cube(instance);
}
void DauQuat() {
	// VẼ các cánh quạt
	CanhQuat(0);
	CanhQuat(72);
	CanhQuat(144);
	CanhQuat(216);
	CanhQuat(288);
	//Đầu quạt
	setColor(vec4(0.096, 0.912, 0.551, 1.0));
	for (int i = 0; i < 180; i += 5)
	{
		instance = model * modelThan * modelDau * Translate(0, 0, dayCanhQuat * 0.5 + 0.075) * RotateZ(i) * Scale(dCanhQuat, hCanhQuat / 4, dayCanhQuat * 2);
		Cube(instance);
	}
	// đầu
	setColor(vec4(0.835, 0.632, 0.270, 1.0));
	for (int i = 0; i < 180; i += 5)
	{
		instance = model * modelThan * Translate(0, 0, -0.05)* modelDau * RotateZ(i) * Scale(0.1, 0.1, 0.15);
		Cube(instance);
	}
}
float hThan = 0.7;
void thanQuat() {
	setColor(vec4(0.265, 0.005, 0.912, 1.0));
	// thân trên
	for (int i = 0; i < 180; i += 10)
	{
		instance = model * modelThan * Translate(0, -hThan * 0.5, -0.1) * RotateY(i) * Scale(0.05, hThan, 0.05);
		Cube(instance);
	}
	// thân duoi
	for (int i = 0; i < 180; i += 10)
	{
		instance = model * Translate(0, -hThan * 0.5, -0.1) * RotateY(i) * Scale(0.05, hThan, 0.05);
		Cube(instance);
	}
	// đáy
	for (int i = 0; i < 180; i += 10)
	{
		instance = model * Translate(0, -hThan, -0.1) * RotateY(i) * Scale(0.15, 0.05, 0.15);
		Cube(instance);
	}
}
int runQuat=1;

void quat() {
	model = Translate(0.5 * wNB, hThan, -0.5 * dNB - 0.5);
	modelThan = Translate(0, nangQuat, 0);
	modelDau = RotateY(quayDauQuat);
	DauQuat();
	thanQuat();
}


void NhaBep() {
	KhungBep();
	TuLanh();
	banAn();
	DayTuTreo();
	bepNau();
	quat();
}
// Dũng End



// Thái and Hải Start

//KHAI BÁO BIẾN TOÀN CỤC
mat4 THuong;
// THÔNG SỐ CỦA PHÒNG NGỦ
GLfloat rongP = 3, caoP = 3.5, dayT = 0.15;
GLfloat daiP = rongP + 2 * dayT;
//THÔNG SỐ CỬA RA VÀO
GLfloat caoCua = 2, rongCua = 1, dayCua = 0.04;
GLfloat moCua = 0;

//THÔNG SỐ CỬA SỔ
GLfloat caoTuongCS = 1.2; //Cửa sổ cách mặt đất
GLfloat rongBoCS = 1.2; //Cả bộ cửa sổ rộng...
GLfloat caoCS = 1, rongCS = rongBoCS / 2, dayCS = dayT / 2;
GLfloat keoCS = 0; //Điều chỉnh kéo cửa sổ

GLfloat dayKhungCua = 0.05;
GLfloat dayKinh = 0.02;
GLfloat dayCanhCuaSo = dayT / 2;

//Cửa sổ nằm cân đều 2 bên tường
GLfloat rongMB = (rongP - rongBoCS) / 2;

void Block(mat4 ins, vec4 color) { //Vẽ 1 khối với ma trận thể hiện và mã màu của nó
	diffuse_product = light_diffuse * color;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_view * THuong * ins);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
GLfloat tranNhaXZ = 7.2, tranNhaY = 0.2;

void tranNha() {
	instance = Translate(0, 0.5 * tranNhaY + caoP, 0) * Scale(tranNhaXZ, tranNhaY, tranNhaXZ);
	colorCode = vec4(0.440, 0.448, 0.485, 1.0); //Mã màu trần nhà
	Block(instance, colorCode);
}
void manhTuong(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz) {
	instance = Translate(x, y, z) * Scale(sx, sy, sz);
	colorCode = vec4(0.701, 0.760, 0.721, 1.0); //Mã màu các bức tường
	Block(instance, colorCode);
}
void xayTuongPhongNgu() {
	//Tường 1: Bên phải, sát với tường phòng khách
	GLfloat daiT1 = daiP - rongCua - dayT;
	manhTuong(0, 0.5 * caoP, -0.5 * daiT1, dayT, caoP, daiT1);
	////Bức nhỏ bên trên cửa ra vào:
	GLfloat caoMN = caoP - caoCua;
	manhTuong(0, 0.5 * caoMN + caoCua, -0.5 * rongCua - daiT1, dayT, caoMN, rongCua);

	//Tường 2: Bên trái, tường bên ngoài
	manhTuong(-(rongP + dayT), 0.5 * caoP, -0.5 * daiP, dayT, caoP, daiP);
	//Tường 3: Tường có cửa sổ: Gồm 2 mảnh bên và 2 mảnh trên dưới
			//Vẽ 2 mảnh bên:
	manhTuong(-(0.5 * rongMB + 0.5 * dayT), 0.5 * caoP, -(0.5 * dayT), rongMB, caoP, dayT);
	manhTuong(-(1.5 * rongMB + 0.5 * dayT + rongBoCS), 0.5 * caoP, -(0.5 * dayT), rongMB, caoP, dayT);
	// Vẽ 2 mảnh trên dưới
		//Mảnh dưới
	manhTuong(-(rongMB + 0.5 * dayT + 0.5 * rongBoCS), 0.5 * caoTuongCS, -(0.5 * dayT), rongBoCS, caoTuongCS, dayT);
	//Mảnh trên
	GLfloat caoMT = caoP - (caoTuongCS + caoCS); //chieu cao cua manh tren
	manhTuong(-(rongMB + 0.5 * dayT + 0.5 * rongBoCS), 0.5 * caoMT + caoTuongCS + caoCS, -(0.5 * dayT), rongBoCS, caoMT, dayT);
	//Tường 4: Tường ngoài

	manhTuong(-(0.5 * daiP - 0.5 * dayT), 0.5 * caoP, -(1.5 * dayT + rongP), daiP, caoP, dayT);
	//Nền nhà:
	instance = Translate(-(0.5 * daiP - 0.5 * dayT), -0.5 * dayT, -(0.5 * daiP)) * Scale(daiP, dayT, daiP);
	colorCode = vec4(0.701, 0.760, 0.721, 1.0); //Mã màu nền nhà
	Block(instance, colorCode);
}
mat4 dichChuyenCua;
GLfloat quayTN = 0, rTN = 0.05, daiTNCua = 0.2;
void tayNamCuaRaVaoDoc() {
	instance = Scale(daiTNCua, rTN, rTN);
	colorCode = vec4(0.701, 0.760, 0.721, 1.0);
	Block(dichChuyenCua * instance, colorCode);
}
void tayNamCuaRaVaoNgang() {
	instance = Translate(0, 0, -0.5 * daiTNCua) * Scale(rTN, rTN, daiTNCua);
	colorCode = vec4(0.701, 0.760, 0.721, 1.0);
	Block(dichChuyenCua * instance, colorCode);
}
void manhCua() {
	instance = Translate(0, 0, 0.5 * rongCua) * Scale(dayCua, caoCua, rongCua);
	colorCode = vec4(0.205, 0.018, 0.030, 1.0); //Mã màu cửa ra vào
	Block(dichChuyenCua * instance, colorCode);
}
void cuaRaVao() {
	dichChuyenCua = Translate(-0.5 * dayCua, 0.5 * caoCua, -daiP + dayT) * RotateY(moCua);
	manhCua();
	dichChuyenCua *= Translate(0, 0, rongCua - 0.08) * RotateX(quayTN);
	tayNamCuaRaVaoDoc();
	dichChuyenCua *= Translate(-0.5 * daiTNCua - 0.5 * rTN, 0, 0.5 * rTN);
	tayNamCuaRaVaoNgang();
	dichChuyenCua *= Translate(daiTNCua + rTN, 0, 0);
	tayNamCuaRaVaoNgang();
}
//VẼ CỬA SỔ
mat4 dichCS;
void manhKhungCS(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz) {
	instance = Translate(x, y, z) * Scale(sx, sy, sz);
	colorCode = vec4(0.205, 0.018, 0.030, 1.0); //Mã màu các thanh khung cửa sổ
	Block(dichCS * instance, colorCode);
}

void khungCS() {
	GLfloat x1 = dayKhungCua, y1 = caoCS, z = dayT;
	// Thanh bên phải khung cửa sổ
	manhKhungCS(-0.5 * x1, 0.5 * y1, -0.5 * z, x1, y1, z);

	// Thanh bên trái khung cửa sổ
	manhKhungCS(0.5 * x1 - rongBoCS, 0.5 * y1, -0.5 * z, x1, y1, z);

	GLfloat x2 = rongBoCS, y2 = dayKhungCua;
	// Thanh bên dưới khung cửa sổ
	manhKhungCS(-0.5 * x2, 0.5 * y2, -0.5 * z, x2, y2, z);

	// Thanh bên dưới khung cửa sổ
	manhKhungCS(-0.5 * x2, -0.5 * y2 + caoCS, -0.5 * z, x2, y2, z);
}

GLfloat xCanhCuaSo = rongBoCS / 2 - dayKhungCua, yCanhCuaSo = caoCS - dayKhungCua * 2, zCanhCuaSo = dayCanhCuaSo;

void canhCS(GLfloat keoCS, GLfloat x, GLfloat y, GLfloat z) {
	dichCS *= Translate(keoCS, 0, 0) * Translate(x, y, z);
	GLfloat x1 = dayKhungCua, y1 = yCanhCuaSo, z1 = dayCanhCuaSo;
	// Thanh bên phải cửa sổ
	manhKhungCS(-0.5 * x1 - dayKhungCua, 0.5 * y1 + dayKhungCua, -0.5 * z, x1, y1, z);

	// Thanh bên trái cửa sổ
	manhKhungCS(-0.5 * x1 - xCanhCuaSo, 0.5 * y1 + dayKhungCua, -0.5 * z, x1, y1, z);

	GLfloat x2 = xCanhCuaSo, y2 = dayKhungCua;
	// Thanh bên dưới cửa sổ
	manhKhungCS(-0.5 * x2 - dayKhungCua, 0.5 * y2 + dayKhungCua, -0.5 * z, x2, y2, z);

	// Thanh bên trên khung cửa sổ
	manhKhungCS(-0.5 * x2 - dayKhungCua, 0.5 * y2 + yCanhCuaSo, -0.5 * z, x2, y2, z);
	GLfloat x3 = xCanhCuaSo - 2 * dayKhungCua + 0.01;
	GLfloat y3 = yCanhCuaSo - dayKhungCua;
	GLfloat z3 = dayKinh;
	instance = Translate(-0.5 * x3 - 2 * dayKhungCua, 0.5 * y3 + 2 * dayKhungCua, -0.5 * dayCanhCuaSo) * Scale(x3, y3, z3);
	colorCode = vec4(0.000, 0.990, 0.962, 1.0);
	Block(dichCS * instance, colorCode);
}
GLfloat keoP = 0, keoT = 0;
void boCuaSo() {
	dichCS = Translate(-rongMB - 0.5 * dayT, caoTuongCS, 0);
	khungCS();
	canhCS(-keoP, 0, 0, 0);
	dichCS = Translate(-rongMB - 0.5 * dayT, caoTuongCS, 0);
	canhCS(-keoT, -xCanhCuaSo, 0, -dayCanhCuaSo);
}


// Vẽ giường ngủ
GLfloat chanGiuongY = 0.3;
GLfloat chanGiuongXZ = 0.05;
//Thông số bề mặt giường
GLfloat daiGiuong = 2.0;
GLfloat rongGiuong = 1.2;
GLfloat dayGiuong = 0.2;
//Thông số miếng gỗ đầu giường:
GLfloat caoDauGiuong = 0.4;

mat4 dichChuyenGiuong;
void chanGiuong(GLfloat x, GLfloat y, GLfloat z)
{
	instance = Translate(x, y, z) * Scale(chanGiuongXZ, chanGiuongY, chanGiuongXZ);
	colorCode = vec4(0.930, 0.609, 0.360, 1.0); //Mã màu chân giường
	Block(dichChuyenGiuong * instance, colorCode);
}
void matGiuong()
{
	instance = Scale(daiGiuong, dayGiuong, rongGiuong);
	colorCode = vec4(0.930, 0.609, 0.360, 1.0); //Mã màu mặt giường
	Block(dichChuyenGiuong * instance, colorCode);
}


void dauGiuong() {
	instance = Translate(-daiGiuong * 0.5 + chanGiuongXZ * 0.5, 0.5 * dayGiuong + 0.5 * caoDauGiuong, 0) * Scale(chanGiuongXZ, caoDauGiuong, rongGiuong);
	colorCode = vec4(0.930, 0.609, 0.360, 1.0); //Mã màu mặt giường
	Block(dichChuyenGiuong * instance, colorCode);
}

void giuong()
{
	dichChuyenGiuong = Translate(-(rongP + 0.5 * dayT) + 0.5 * daiGiuong, chanGiuongY + 0.5 * dayGiuong, -dayT - 0.5 * rongGiuong);
	matGiuong();
	dauGiuong();
	//chan ban 1
	chanGiuong(-daiGiuong * 0.5 + chanGiuongXZ * 0.5, -dayGiuong * 0.5 - chanGiuongY * 0.5, -rongGiuong * 0.5 + chanGiuongXZ * 0.5);
	chanGiuong(-daiGiuong * 0.5 + chanGiuongXZ * 0.5, -dayGiuong * 0.5 - chanGiuongY * 0.5, rongGiuong * 0.5 - chanGiuongXZ * 0.5);
	chanGiuong(daiGiuong * 0.5 - chanGiuongXZ * 0.5, -dayGiuong * 0.5 - chanGiuongY * 0.5, -rongGiuong * 0.5 + chanGiuongXZ * 0.5);
	chanGiuong(daiGiuong * 0.5 - chanGiuongXZ * 0.5, -dayGiuong * 0.5 - chanGiuongY * 0.5, rongGiuong * 0.5 - chanGiuongXZ * 0.5);

}



mat4 dichChuyenBan;

GLfloat matBanX = 0.6;
GLfloat matBanY = 0.08;
GLfloat matBanZ = 0.8;
void matBan()
{
	instance = Scale(matBanX, matBanY, matBanZ);
	colorCode = vec4(0.465, 0.304, 0.180, 1.0); //Mã màu mặt bàn
	Block(dichChuyenBan * instance, colorCode);
}
GLfloat chanBanY = 0.8;
GLfloat chanBanXZ = 0.05;
GLfloat keoNgan = 0;
GLfloat l = 0;
GLfloat r = 0;
void chanBan(GLfloat x, GLfloat y, GLfloat z)
{
	instance = Translate(x, y, z) * Scale(chanBanXZ, chanBanY, chanBanXZ);
	colorCode = vec4(0.465, 0.304, 0.180, 1.0); //Mã màu chân bàn
	Block(dichChuyenBan * instance, colorCode);
}
//Vẽ tủ
//độ dày của 1 mảnh ngăn kéo:
GLfloat dayManhNK = 0.02;
GLfloat matTS = matBanZ / 2 - chanBanXZ;
GLfloat matTP = matBanX - 2 * dayManhNK;
GLfloat nganKeoY = 0.2;

mat4 dcNganKeo;
void manhNganKeo(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz)
{
	instance = Translate(x, y, z) * Scale(sx, sy, sz);
	colorCode = vec4(0.885, 0.637, 0.131, 1.0); //Mã màu ngăn kéo
	Block(dichChuyenBan * dcNganKeo * instance, colorCode);
}
//Thông số tay nắm:
GLfloat caoTN = 0.03, daiTN = 0.07;
void tayNamNganKeo() {
	instance = Translate(0.5 * matTP + dayManhNK + 0.5 * caoTN, 0, 0) * Scale(caoTN, caoTN, daiTN);
	colorCode = vec4(0.912, 0.137, 0.250, 1.0); //Mã màu tay nắm
	Block(dichChuyenBan * dcNganKeo * instance, colorCode);
}
void nganKeo() {
	manhNganKeo(0.5 * matTP + 0.5 * dayManhNK, 0, 0, dayManhNK, nganKeoY, matTS); //Manh truoc
	manhNganKeo(-(0.5 * matTP + 0.5 * dayManhNK), 0, 0, dayManhNK, nganKeoY, matTS); //Manh sau
	manhNganKeo(0, 0, 0.5 * matTS - 0.5 * dayManhNK, matTP, nganKeoY, dayManhNK);
	manhNganKeo(0, 0, -0.5 * matTS + 0.5 * dayManhNK, matTP, nganKeoY, dayManhNK);
	manhNganKeo(0, -0.5 * nganKeoY - 0.5 * dayManhNK, 0, matTP + 2 * dayManhNK, dayManhNK, matTS); //Manh duoi
	tayNamNganKeo();

}
void nganKeoT() {
	dcNganKeo = Translate(l, 0, 0) * Translate(0, -0.5 * matBanY - 0.5 * nganKeoY, 0.5 * matTS);
	nganKeo();
}
void nganKeoP() {
	dcNganKeo = Translate(r, 0, 0) * Translate(0, -0.5 * matBanY - 0.5 * nganKeoY, -0.5 * matTS);
	nganKeo();
}
GLfloat caoTu = chanBanY - nganKeoY - dayManhNK - 0.05; //Chiều cao của tủ phía dưới, cách mặt đất 5 cm, mảnh trước sau.
GLfloat sauTu = matBanZ - 2 * chanBanXZ; //chiều dài của mảnh phía sau tủ
GLfloat caoTuTP = chanBanY;
GLfloat rongTuTP = matBanX - 2 * chanBanXZ; //chiều rộng của mảnh bên trái và bên phải của tủ
GLfloat dayTu = dayManhNK;
GLfloat moCuaTuT = 0, moCuaTuP = 0;

void manhTu(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz)
{
	instance = Translate(x, y, z) * Scale(sx, sy, sz);
	colorCode = vec4(0.700, 0.458, 0.271, 1.0); //Mã màu tủ mở bên dưới
	Block(dichChuyenBan * instance, colorCode);
}
mat4 moTu;
void tayNamCuaTu(GLfloat z) {
	instance = Translate(0, 0, z) * Scale(caoTN, daiTN, caoTN);
	colorCode = vec4(0.912, 0.137, 0.250, 1.0); //Mã màu tay nắm
	Block(dichChuyenBan * moTu * instance, colorCode);
}
void cuaTuT() {
	instance = Translate(0, 0, -0.5 * matTS) * Scale(dayManhNK, caoTu, matTS);
	moTu = Translate(0.5 * matBanX - 0.5 * dayManhNK, -matBanY * 0.5 - nganKeoY - dayManhNK - 0.5 * caoTu, 0) * Translate(0, 0, matTS) * RotateY(moCuaTuT);
	colorCode = vec4(0.700, 0.458, 0.271, 1.0); //Mã màu cửa tủ
	Block(dichChuyenBan * moTu * instance, colorCode);
	moTu *= Translate(0.5 * dayTu + 0.5 * caoTN, 0, -2 * matTS / 3);
	tayNamCuaTu(-0.5 * caoTN);
}

void cuaTuP() {
	instance = Translate(0, 0, 0.5 * matTS) * Scale(dayManhNK, caoTu, matTS);
	moTu = Translate(0.5 * matBanX - 0.5 * dayManhNK, -matBanY * 0.5 - nganKeoY - dayManhNK - 0.5 * caoTu, 0) * Translate(0, 0, -matTS) * RotateY(moCuaTuP);
	colorCode = vec4(0.700, 0.458, 0.271, 1.0); //Mã màu cửa tủ
	Block(dichChuyenBan * moTu * instance, colorCode);
	moTu *= Translate(0.5 * dayTu + 0.5 * caoTN, 0, 2 * matTS / 3);
	tayNamCuaTu(0.5 * caoTN);
}

void Tu()
{
	manhTu(0, -matBanY * 0.5 - caoTuTP * 0.5, matTS + dayTu * 0.5, rongTuTP, caoTuTP, dayTu); //Manh trai
	manhTu(0, -matBanY * 0.5 - caoTuTP * 0.5, -(matTS + dayTu * 0.5), rongTuTP, caoTuTP, dayTu); //Manh phai
	manhTu(-0.5 * matBanX + 0.5 * dayTu, -(0.5 * matBanY + nganKeoY + dayManhNK) - 0.5 * (caoTu + 0.05), 0, dayTu, caoTu + 0.05, sauTu); //Manh sau 
	manhTu(0, -(0.5 * matBanY + nganKeoY + dayManhNK + caoTu + 0.5 * dayTu), 0, matBanX, dayTu, sauTu);
	cuaTuT();
	cuaTuP();
}
void Ban()
{
	dichChuyenBan = Translate(-(rongP + 0.5 * dayT) + 0.5 * matBanX, chanBanY + 0.5 * matBanY, -dayT - rongGiuong - 0.5 * matBanZ - 0.15);
	matBan();
	chanBan(-matBanX * 0.5 + chanBanXZ * 0.5, -matBanY * 0.5 - chanBanY * 0.5, -matBanZ * 0.5 + chanBanXZ * 0.5);
	chanBan(-matBanX * 0.5 + chanBanXZ * 0.5, -matBanY * 0.5 - chanBanY * 0.5, matBanZ * 0.5 - chanBanXZ * 0.5);
	chanBan(matBanX * 0.5 - chanBanXZ * 0.5, -matBanY * 0.5 - chanBanY * 0.5, -matBanZ * 0.5 + chanBanXZ * 0.5);
	chanBan(matBanX * 0.5 - chanBanXZ * 0.5, -matBanY * 0.5 - chanBanY * 0.5, matBanZ * 0.5 - chanBanXZ * 0.5);

	nganKeoT();
	nganKeoP();
	Tu();

}

// Vẽ ghế
GLfloat chanGheY = 0.45;
GLfloat chanGheXZ = 0.05;
//Thông số bề mặt ghế
GLfloat matGheXZ = 0.5;
GLfloat dayGhe = 0.04;
//Thông số lưng ghế
GLfloat caoLungGhe = 0.5;
//Miếng gỗ ngang lưng ghế:
GLfloat dayMN = 0.02;
GLfloat yMN = 0.1;
mat4 dichChuyenGhe;
void chanGhe(GLfloat x, GLfloat y, GLfloat z)
{
	instance = Translate(x, y, z) * Scale(chanGheXZ, chanGheY, chanGheXZ);
	colorCode = vec4(0.912, 0.826, 0.150, 1.0); //Mã màu ghế
	Block(dichChuyenGhe * instance, colorCode);
}
void matGhe()
{
	instance = Scale(matGheXZ, dayGhe, matGheXZ);
	colorCode = vec4(0.912, 0.826, 0.150, 1.0); //Mã màu ghế
	Block(dichChuyenGhe * instance, colorCode);
}

void lungGhe() {
	dichChuyenGhe *= Translate(0.5 * matGheXZ - 0.5 * chanGheXZ, 0.5 * caoLungGhe, 0);
	//thanh lưng ghế: thanh bên trái
	instance = Translate(0, 0, -0.5 * matGheXZ + 0.5 * chanGheXZ) * Scale(chanGheXZ, caoLungGhe, chanGheXZ);
	colorCode = vec4(0.912, 0.826, 0.150, 1.0); //Mã màu mặt giường
	Block(dichChuyenGhe * instance, colorCode);
	//thanh lưng ghế: thanh bên trái
	instance = Translate(0, 0, 0.5 * matGheXZ - 0.5 * chanGheXZ) * Scale(chanGheXZ, caoLungGhe, chanGheXZ);
	colorCode = vec4(0.912, 0.826, 0.150, 1.0); //Mã màu mặt giường
	Block(dichChuyenGhe * instance, colorCode);

	//Mảnh ngang lưng ghế
	instance = Translate(0, 0.5 * caoLungGhe - 0.5 * yMN, 0) * Scale(dayMN, yMN, matGheXZ);
	colorCode = vec4(0.912, 0.826, 0.150, 1.0); //Mã màu mặt giường
	Block(dichChuyenGhe * instance, colorCode);

	//Mảnh ngang lưng ghế
	instance = Translate(0, 0, 0) * Scale(dayMN, yMN, matGheXZ);
	colorCode = vec4(0.912, 0.826, 0.150, 1.0); //Mã màu mặt giường
	Block(dichChuyenGhe * instance, colorCode);
}
GLfloat keoGhe = 0;
void ghe() {
	dichChuyenGhe = Translate(keoGhe, 0, 0) * Translate(-(rongP + 0.5 * dayT) + matBanX + 0.5 * matGheXZ, chanGheY + 0.5 * dayGhe, -dayT - rongGiuong - 0.5 * matBanZ - 0.15);
	matGhe();
	chanGhe(-matGheXZ * 0.5 + chanGheXZ * 0.5, -dayGhe * 0.5 - chanGheY * 0.5, -matGheXZ * 0.5 + chanGheXZ * 0.5);
	chanGhe(-matGheXZ * 0.5 + chanGheXZ * 0.5, -dayGhe * 0.5 - chanGheY * 0.5, matGheXZ * 0.5 - chanGheXZ * 0.5);
	chanGhe(matGheXZ * 0.5 - chanGheXZ * 0.5, -dayGhe * 0.5 - chanGheY * 0.5, -matGheXZ * 0.5 + chanGheXZ * 0.5);
	chanGhe(matGheXZ * 0.5 - chanGheXZ * 0.5, -dayGhe * 0.5 - chanGheY * 0.5, matGheXZ * 0.5 - chanGheXZ * 0.5);
	lungGhe();

}

//VẼ TỦ QUẦN ÁO
mat4 dichChuyenTu;

GLfloat caoTuQA = 1.8, dayManhTu = dayManhNK, rongTuQA = 1.2 + 2 * dayManhTu, dayTuQA = 0.5, caoTuQADuoi = 1.2, caoTuQATren = caoTuQA - caoTuQADuoi - dayManhTu;
void manhTuQ(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz)
{
	instance = Translate(x, y, z) * Scale(sx, sy, sz);
	colorCode = vec4(0.912, 0.657, 0.066, 1.0); //Mã màu tủ quần áo
	Block(dichChuyenTu * instance, colorCode);
}
mat4 moTuQA;
GLfloat moCuaTuQAT = 0, moCuaTuQAP = 0;
void tayNamCuaTuQA(GLfloat x) {
	instance = Translate(x, 0, 0) * Scale(caoTN, daiTN, caoTN);
	colorCode = vec4(0.912, 0.137, 0.250, 1.0); //Mã màu tay nắm
	Block(dichChuyenTu * moTuQA * instance, colorCode);
}
void cuaTuQAT() {
	GLfloat x1 = 0.5 * rongTuQA, y1 = 1.5 * dayManhTu + caoTuQATren;
	instance = Translate(0.5 * x1, 0, 0) * Scale(x1, y1, dayManhTu);
	moTuQA = Translate(-x1, -0.5 * y1 + 0.5 * caoTuQA + 0.5 * dayManhTu, 0.5 * dayTuQA + 0.5 * dayManhTu) * RotateY(moCuaTuQAT);
	colorCode = vec4(0.912, 0.749, 0.458, 1.0); //Mã màu cửa tủ
	Block(dichChuyenTu * moTuQA * instance, colorCode);
	moTuQA *= Translate(2 * x1 / 3, 0, 0.5 * caoTN + dayManhTu);
	tayNamCuaTuQA(0.5 * caoTN);
}
void cuaTuQAP() {
	GLfloat x1 = 0.5 * rongTuQA, y1 = 1.5 * dayManhTu + caoTuQATren;
	instance = Translate(-0.5 * x1, 0, 0) * Scale(x1, y1, dayManhTu);
	moTuQA = Translate(x1, -0.5 * y1 + 0.5 * caoTuQA + 0.5 * dayManhTu, 0.5 * dayTuQA + 0.5 * dayManhTu) * RotateY(moCuaTuQAP);
	colorCode = vec4(0.912, 0.749, 0.458, 1.0); //Mã màu cửa tủ
	Block(dichChuyenTu * moTuQA * instance, colorCode);
	moTuQA *= Translate(-2 * x1 / 3, 0, 0.5 * caoTN + dayManhTu);
	tayNamCuaTuQA(-0.5 * caoTN);
}
GLfloat keoCTT = 0, keoCTP = 0;
void canhCuaKeoTuT() {
	GLfloat x1 = rongTuQA / 2, y1 = caoTuQADuoi, z1 = dayManhTu / 2;
	instance = Scale(x1, y1, z1);
	mat4 model = Translate(keoCTT, 0, 0) * Translate(-0.5 * x1, -0.5 * caoTuQA + 0.5 * y1, 0.5 * dayTuQA + 0.5 * z1);
	colorCode = vec4(0.912, 0.749, 0.458, 1.0);
	Block(dichChuyenTu * model * instance, colorCode);
	model *= Translate(-x1 / 3, 0, 0) * Scale(caoTN, daiTN, caoTN);
	colorCode = vec4(0.912, 0.137, 0.250, 1.0); //Mã màu tay nắm
	Block(dichChuyenTu * model, colorCode);
}
void canhCuaKeoTuP() {
	GLfloat x1 = rongTuQA / 2, y1 = caoTuQADuoi, z1 = dayManhTu / 2;
	instance = Scale(x1, y1, z1);
	mat4 model = Translate(keoCTP, 0, 0) * Translate(0.5 * x1, -0.5 * caoTuQA + 0.5 * y1, 0.5 * dayTuQA + 1.5 * z1);
	colorCode = vec4(0.912, 0.749, 0.458, 1.0);
	Block(dichChuyenTu * model * instance, colorCode);
	model *= Translate(x1 / 3, 0, 0) * Scale(caoTN, daiTN, caoTN);
	colorCode = vec4(0.912, 0.137, 0.250, 1.0); //Mã màu tay nắm
	Block(dichChuyenTu * model, colorCode);
}
void tuQuanAo() {
	dichChuyenTu = Translate(-(rongP + 0.5 * dayT) + 0.5 * rongTuQA, 0.5 * caoTuQA + dayManhTu, -(dayT + rongP - 0.5 * dayTuQA));
	GLfloat rongMB = dayTuQA - dayManhTu;
	manhTuQ(0, 0.5 * caoTuQA + 0.5 * dayManhTu, 0, rongTuQA, dayManhTu, dayTuQA); //mảnh trên đầu
	manhTuQ(0, -(0.5 * caoTuQA + 0.5 * dayManhTu), 0, rongTuQA, dayManhTu, dayTuQA); //mảnh phía dưới
	manhTuQ(0, 0, -0.5 * dayTuQA + 0.5 * dayManhTu, rongTuQA, caoTuQA, dayManhTu); //mảnh phía sau
	manhTuQ(-0.5 * rongTuQA + 0.5 * dayManhTu, 0, 0.5 * dayTuQA - 0.5 * rongMB, dayManhTu, caoTuQA, dayTuQA - dayManhTu); //mảnh bên trái
	manhTuQ(0.5 * rongTuQA - 0.5 * dayManhTu, 0, 0.5 * dayTuQA - 0.5 * rongMB, dayManhTu, caoTuQA, dayTuQA - dayManhTu); //mảnh bên phải
	manhTuQ(0, 0, 0, dayManhTu, caoTuQA, dayTuQA - dayManhTu); // mảnh đứng ở giữa
	manhTuQ(0, 0.5 * caoTuQATren, 0, rongTuQA, dayManhTu, dayTuQA);
	cuaTuQAT();
	cuaTuQAP();
	canhCuaKeoTuT();
	canhCuaKeoTuP();

}
//Vẽ quạt góc nhà
GLfloat quayCanhQuat = 0;
GLfloat daiCanh = 0.3;
GLfloat dayCanh = 0.01;
GLfloat rongCanh = 0.05;
mat4 dichChuyenQuat;
void canh(GLfloat rx) {
	instance = RotateX(rx) * Translate(0, 0.5 * daiCanh, 0) * Scale(dayCanh, daiCanh, rongCanh);
	colorCode = vec4(0.910, 0.091, 0.117, 1.0); //Mã màu cánh quạt
	Block(dichChuyenQuat * instance, colorCode);

}

void _3CanhQuat() {

	canh(quayCanhQuat);
	canh(quayCanhQuat + 72);
	canh(quayCanhQuat + 144);
	canh(quayCanhQuat + 216);
	canh(quayCanhQuat + 288);
}
GLfloat daiTruNgang = 0.1;
GLfloat caoTruDoc = 0.8;
GLfloat banKinhDe = 0.4;
GLfloat dayDeQuat = 0.03;
void quat2() {
	dichChuyenQuat = Translate(-0.7, 0.5 * rongCanh + caoTruDoc + dayDeQuat, -0.6);
	_3CanhQuat();

	//Vẽ trụ ngang
	instance = Scale(daiTruNgang, rongCanh, rongCanh);
	dichChuyenQuat *= Translate(0.5 * dayCanh + 0.5 * daiTruNgang, 0, 0);
	colorCode = vec4(0.743, 0.910, 0.241, 1.0); //Mã trụ quạt
	Block(dichChuyenQuat * instance, colorCode);

	// Vẽ trụ dọc
	instance = Scale(rongCanh, caoTruDoc, rongCanh);
	dichChuyenQuat *= Translate(0.5 * daiTruNgang + 0.5 * rongCanh, -0.5 * caoTruDoc + 0.5 * rongCanh, 0);
	colorCode = vec4(0.743, 0.910, 0.241, 1.0); //Mã trụ quạt
	Block(dichChuyenQuat * instance, colorCode);
	//Vẽ đế quạt
	dichChuyenQuat *= Translate(0, -0.5 * caoTruDoc - 0.5 * dayDeQuat, 0);
	for (int i = 0; i <= 180; i++) {
		instance = RotateY(i) * Scale(0.01, dayDeQuat, banKinhDe);
		colorCode = vec4(0.910, 0.481, 0.092, 1.0); //Mã màu đế quạt
		Block(dichChuyenQuat * instance, colorCode);
	}

}
void phongNgu() {
	THuong = Translate(0, 0, 0);
	xayTuongPhongNgu();
	boCuaSo();
	cuaRaVao();
	giuong();
	Ban();
	tuQuanAo();
	ghe();
	quat2();
}
void phongNgu1() {
	THuong = Translate(0, 0, -daiP + dayT);
	xayTuongPhongNgu();
	//boCuaSo();
	cuaRaVao();
	giuong();
	Ban();
	tuQuanAo();
	ghe();
	quat2();
}
// Thái and Hải End



// Phát Start

class MatrixStack {
public:
	const static int MAX = 50;
	MatrixStack() { index = 0; }
	void push(const mat4& matrix);
	mat4 pop();
private:
	int index;
	mat4 matrices[MAX];
};

MatrixStack modelStack;

void MatrixStack::push(const mat4& matrix) {
	matrices[index] = matrix;
	index++;
}

mat4 MatrixStack::pop() {
	index--;
	return matrices[index];
}


void createElement(mat4 ins) {
	modelStack.push(mdl);
	mdl *= ins;
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mdl);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	mdl = modelStack.pop();
}

// định nghĩa màu vật liệu
color4 colorWood = color4(0.205, 0.018, 0.030, 1.0);
color4 colorGlass = color4(0.000, 0.990, 0.962, 0.5);
color4 colorWall = color4(0.701, 0.760, 0.721, 1.0);
color4 colorPlasticWhite = color4(0.956, 0.975, 0.936, 1.0);
color4 colorPlasticBlack = color4(0.000, 0.000, 0.000, 1.0);
color4 colorSofa = color4(0.6, 0.6, 0.6, 1.0);
color4 colorGrass = color4(0.033, 1.000, 0.024, 1.0);
color4 colorScreenoff = color4(0.000, 0.000, 0.000, 0.8);
color4 colorScreen = colorScreenoff;

// định nghĩa độ dày vật liệu
GLfloat dayTuong = 0.15;
GLfloat dayCua1 = 0.1;
GLfloat dayKinh1 = 0.04;
GLfloat dayCanhCS = dayTuong / 2;
GLfloat dayBac = 0.06;
GLfloat dayTu1 = 0.01;
GLfloat dayTayNamTu = 0.01;
GLfloat duongKinhTayNam = 0.03;
GLfloat dayVienTivi = 0.008;
GLfloat dayChanGheSofa = 0.03;

/// <summary>
/// VẼ TƯỜNG NHÀ
/// </summary>

// định nghĩa thông số 
// sàn
GLfloat xSan = 3 + 2 * dayTuong, ySan = 0.2, zSan = 4.5 + dayTuong;

// tường trước
GLfloat xTuongTruoc = 3.3, yTuongTruoc = 3.5, zTuongTruoc = dayTuong;
GLfloat xCuaTruoc = 2, yCuaTruoc = 2.5;

// tường phải
GLfloat xTuongPhai = dayTuong, yTuongPhai = 3.5, zTuongPhai = 3.5 + 2 * dayTuong;
GLfloat xCuaSoTP = dayTuong, yCuaSoTP = 1, zCuaSoTP = 1.2;
GLfloat cuaCachDat = 1.2;

// tường trái
GLfloat xTuongTrai = dayTuong, yTuongTrai = 3.5, zTuongTrai = 2 + dayTuong;

// tường sau
GLfloat xTuongSau = 2 + dayTuong, yTuongSau = 3.5, zTuongSau = dayTuong;
GLfloat xBac = xTuongSau - dayTuong, yBac = 0.62, zBac = 0.5;

void sanPK() {
	setColor(colorWall);
	ctm = Translate(xSan * 0.5, -ySan * 0.5, -zSan * 0.5 + 1 - dayTuong) * Scale(xSan, ySan, zSan);
	// Tạo sàn nhà phòng khách
	createElement(ctm);
}

void tuongTruocPK() {
	setColor(colorWall);

	GLfloat x1 = (xTuongTruoc - xCuaTruoc) / 2;
	GLfloat y1 = yTuongTruoc;
	GLfloat z = zTuongTruoc;
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	// Tạo phần bên trái của tường trước phòng khách
	createElement(ctm);

	ctm = Translate(x1 * 0.5 + x1 + xCuaTruoc, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	// Tạo phần bên phải của tường trước phòng khách
	createElement(ctm);

	GLfloat x2 = xCuaTruoc;
	GLfloat y2 = yTuongTruoc - yCuaTruoc;
	ctm = Translate(x1 + x2 * 0.5, yCuaTruoc + y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	// tạo phần trên của tường trước phòng khách
	createElement(ctm);
}

void tuongPhaiPK() {
	setColor(colorWall);

	GLfloat x = xTuongPhai;
	GLfloat y1 = (yTuongPhai - yCuaSoTP) / 2;
	GLfloat z1 = zTuongPhai;
	// Tạo tường dưới của tường bên phải
	ctm = Translate(xSan - x * 0.5, y1 * 0.5, -z1 * 0.5) * Scale(x, y1, z1);
	createElement(ctm);

	// Tạo tường trên của tường bên phải
	ctm = Translate(xSan - x * 0.5, yTuongPhai - y1 * 0.5, -z1 * 0.5) * Scale(x, y1, z1);
	createElement(ctm);

	GLfloat y2 = yCuaSoTP;
	GLfloat z2 = (zTuongPhai - zCuaSoTP) / 2;
	// Tạo tường trái của tường bên phải 
	ctm = Translate(xSan - x * 0.5, y1 + y2 * 0.5, -z2 * 0.5) * Scale(x, y2, z2);
	createElement(ctm);

	// Tạo tường phải của tường bên phải 
	ctm = Translate(xSan - x * 0.5, y1 + y2 * 0.5, -(z2 * 0.5 + z2 + zCuaSoTP)) * Scale(x, y2, z2);
	createElement(ctm);
}

void tuongTraiPK() {
	setColor(colorWall);
	ctm = Translate(xTuongTrai * 0.5, yTuongTrai * 0.5, -zTuongTrai * 0.5) * Scale(xTuongTrai, yTuongTrai, zTuongTrai);
	createElement(ctm);
}

void tuongSauPK() {
	setColor(colorWall);
	ctm = Translate(xSan - xTuongSau * 0.5, yTuongSau * 0.5, -zSan + zTuongSau * 0.5 + 1 - dayTuong) * Scale(xTuongSau, yTuongSau, zTuongSau);
	createElement(ctm);

	// vẽ bậc để ti vi

	modelStack.push(mdl);

	mdl *= Translate(xSan - dayTuong - xBac, 0, -zSan + 1 + zBac);
	// cạnh trên bậc
	GLfloat x1 = xBac, y1 = dayBac, z = zBac;
	ctm = Translate(x1 * 0.5, yBac - y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh dưới bậc
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh trái bậc
	GLfloat x2 = dayBac, y2 = yBac;
	ctm = Translate(0.5 * x2, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// cạnh phải bậc
	ctm = Translate(xBac - 0.5 * x2, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	mdl = modelStack.pop();
}

void tuongPhongKhach(GLfloat tX, GLfloat tY, GLfloat tZ) {
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	sanPK();

	tuongTruocPK();
	tuongPhaiPK();
	tuongSauPK();

	mdl = modelStack.pop();
}

/// <summary>
/// VẼ CỬA TRƯỚC
/// </summary>

// định nghĩa thông số 
// 
// góc mở cánh cửa
GLfloat openDoor = 0;

// khung trên 
GLfloat xKhungTren = (xCuaTruoc - 2 * dayKhungCua) / 4, yKhungTren = 0.3, zKhungTren = 0.1;

// cánh cửa
GLfloat xCanhCuaTruoc = (xCuaTruoc - dayKhungCua * 2) / 2, yCanhCuaTruoc = yCuaTruoc - dayKhungCua - yKhungTren, zCanhCuaTruoc = dayCua1;


void khungCuaTruoc() {
	setColor(colorWood);
	GLfloat x1 = dayKhungCua, y1 = yCuaTruoc, z = dayTuong;
	// Thanh bên trái khung cửa
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// Thanh bên phải khung cửa
	ctm = Translate(xCuaTruoc - x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	GLfloat x2 = xCuaTruoc, y2 = dayKhungCua;
	// Thanh bên trên khung cửa
	ctm = Translate(x2 * 0.5, yCuaTruoc - y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);
}


void khungTrenCuaTruoc(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	GLfloat x1 = dayKhungCua, y1 = yKhungTren, z = zKhungTren;
	// cạnh trái khung trên
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh phải khung trên
	ctm = Translate(xKhungTren - x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh dưới khung trên
	GLfloat x2 = xKhungTren, y2 = dayKhungCua;
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// cạnh trên khung trên
	ctm = Translate(x2 * 0.5, yKhungTren - y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	setColor(colorGlass);
	GLfloat x3 = xKhungTren - 2 * x1, y3 = yKhungTren - 2 * y2, z3 = dayKinh1;
	ctm = Translate(x3 * 0.5 + x1, y3 * 0.5 + y2, -z * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	mdl = modelStack.pop();
}

void canhCuaTruoc(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	GLfloat x1 = xCanhCuaTruoc, y1 = yCanhCuaTruoc, z1 = zCanhCuaTruoc;
	// cánh trái
	ctm = Translate(0, y1 * 0.5, -z1 * 0.5) * RotateY(-openDoor) * Translate(x1 * 0.5, 0, 0) * Scale(x1, y1, z1);
	createElement(ctm);

	// cạnh phải
	ctm = Translate(2 * x1, y1 * 0.5, -z1 * 0.5) * RotateY(openDoor) * Translate(-x1 * 0.5, 0, 0) * Scale(x1, y1, z1);
	createElement(ctm);

	mdl = modelStack.pop();
}

void cuaTruoc(GLfloat tX, GLfloat tY, GLfloat tZ) {
	modelStack.push(mdl);

	mdl *= Translate(tX, tY, tZ);
	khungCuaTruoc();

	for (int i = 0; i < 4; i++) {
		khungTrenCuaTruoc(dayKhungCua + xKhungTren * i, yCuaTruoc - yKhungTren - dayKhungCua, 0);
	}

	canhCuaTruoc(dayKhungCua, 0, 0);

	mdl = modelStack.pop();
}

/// <summary>
/// VẼ CỬA SỔ TƯỜNG PHẢI
/// </summary>

// định nghĩa thông số 
// 
// điều chỉnh mở cửa
GLfloat openWindow = 0;

// cánh cửa sổ
GLfloat xCanhCS = zCuaSoTP / 2 - dayKhungCua * 2 + dayCanhCS, yCanhCS = yCuaSoTP - dayKhungCua * 2, zCanhCS = dayCanhCS;

void khungCuaSo() {
	setColor(colorWood);
	GLfloat x1 = dayKhungCua, y1 = yCuaSoTP, z = dayTuong;
	// Thanh bên trái khung cửa sổ
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// Thanh bên phải khung cửa sổ
	ctm = Translate(zCuaSoTP - x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	GLfloat x2 = zCuaSoTP, y2 = dayKhungCua;
	// Thanh bên trên khung cửa sổ
	ctm = Translate(x2 * 0.5, yCuaSoTP - y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// Thanh bên dưới khung cửa sổ
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);
}

void canhCuaSo(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	// khung trên cánh cửa sổ
	GLfloat x1 = xCanhCS, y1 = dayCanhCS, z = dayCanhCS;
	ctm = Translate(x1 * 0.5, yCanhCS - y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// khung dưới cánh cửa sổ
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// khung trái cánh cửa sổ
	GLfloat x2 = dayCanhCS, y2 = yCanhCS;
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// khung phải cánh cửa sổ
	ctm = Translate(xCanhCS - x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	setColor(colorGlass);
	GLfloat x3 = xCanhCS - 2 * x2, y3 = yCanhCS - 2 * y1, z3 = dayKinh1;
	ctm = Translate(x3 * 0.5 + x2, y3 * 0.5 + y1, -z * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	mdl = modelStack.pop();
}


void cuaSo(GLfloat tX, GLfloat tY, GLfloat tZ) {
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	khungCuaSo();

	canhCuaSo(dayKhungCua + openWindow, dayKhungCua, 0);
	canhCuaSo(zCuaSoTP - dayKhungCua - xCanhCS - openWindow, dayKhungCua, -dayCanhCS);

	mdl = modelStack.pop();
}

/// <summary>
/// VẼ TỦ TI VI
/// </summary>

// định nghĩa thông số

GLfloat openCabinet = 0; // tủ đóng mở
GLfloat keoTu = 0; // tủ kéo

// tủ kéo nhỏ
GLfloat xTuKeoNho = (xBac - 2 * dayBac) / 6, yTuKeoNho = 0.14, zTuKeoNho = zBac;

// tủ kéo to
GLfloat xTuKeoTo = 2 * xTuKeoNho, yTuKeoTo = (yBac - dayBac * 2) / 3, zTuKeoTo = zBac;

// tủ đóng mở
GLfloat xTuDongMo = 2 * xTuKeoNho, yTuDongMo = (yBac - dayBac * 2) - yTuKeoNho, zTuDongMo = zBac;

void tuKeoNho(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	//cạnh dưới tủ kéo nhỏ
	GLfloat x1 = xTuKeoNho, y1 = dayTu1, z = zTuKeoNho;
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	//cạnh trái tủ kéo nhỏ
	GLfloat x2 = dayTu1, y2 = yTuKeoNho;
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// cạnh phải tủ kéo nhỏ
	ctm = Translate(xTuKeoNho - x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	GLfloat x3 = xTuKeoNho, y3 = yTuKeoNho, z3 = dayTu1;
	// cạnh trước tủ kéo nhỏ
	ctm = Translate(x3 * 0.5, y3 * 0.5, -z3 * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	// cạnh sau tủ kéo nhỏ
	ctm = Translate(x3 * 0.5, y3 * 0.5, -zTuKeoNho + z3 * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	setColor(colorPlasticWhite);
	// trụ tay nắm cửa tủ
	GLfloat x4 = dayTayNamTu, y4 = dayTayNamTu, z4 = 0.015;
	ctm = Translate(x1 * 0.5, y2 * 0.5, z4 * 0.5) * Scale(x4, y4, z4);
	createElement(ctm);

	// tay nắm cửa tủ
	GLfloat x5 = duongKinhTayNam / (sqrt(2)), y5 = x5, z5 = 0.005;
	for (int i = 0; i < 90; i++) {
		ctm = Translate(x1 * 0.5, y2 * 0.5, z4 + z5 * 0.5) * RotateZ(i) * Scale(x5, y5, z5);
		createElement(ctm);
	}

	mdl = modelStack.pop();
}

void tuKeoTo(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	//cạnh dưới tủ kéo to
	GLfloat x1 = xTuKeoTo, y1 = dayTu1, z = zTuKeoTo;
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	//cạnh trái tủ kéo to
	GLfloat x2 = dayTu1, y2 = yTuKeoTo;
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// cạnh phải tủ kéo to
	ctm = Translate(xTuKeoTo - x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	GLfloat x3 = xTuKeoTo, y3 = yTuKeoTo, z3 = dayTu1;
	// cạnh trước tủ kéo to
	ctm = Translate(x3 * 0.5, y3 * 0.5, -z3 * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	// cạnh sau tủ kéo to
	ctm = Translate(x3 * 0.5, y3 * 0.5, -zTuKeoTo + z3 * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	setColor(colorPlasticWhite);
	// trụ tay nắm cửa tủ
	GLfloat x4 = dayTayNamTu, y4 = dayTayNamTu, z4 = 0.015;
	ctm = Translate(x1 * 0.5, y2 * 0.5, z4 * 0.5) * Scale(x4, y4, z4);
	createElement(ctm);

	// tay nắm cửa tủ
	GLfloat x5 = duongKinhTayNam / (sqrt(2)), y5 = x5, z5 = 0.005;
	for (int i = 0; i < 90; i++) {
		ctm = Translate(x1 * 0.5, y2 * 0.5, z4 + z5 * 0.5) * RotateZ(i) * Scale(x5, y5, z5);
		createElement(ctm);
	}

	mdl = modelStack.pop();
}

void tuDongMo(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	// cạnh trên tủ đóng mở
	GLfloat x1 = xTuDongMo, y1 = dayTu1, z = zTuDongMo;
	ctm = Translate(x1 * 0.5, yTuDongMo - y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh dưới tủ đóng mở
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh trái tủ đóng mở
	GLfloat x2 = dayTu1, y2 = yTuDongMo;
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// cạnh phải
	ctm = Translate(xTuDongMo - x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// cửa trái tủ đóng mở
	modelStack.push(mdl);

	mdl *= Translate(dayTu1, dayTu1, 0) * RotateY(-openCabinet);
	GLfloat x3 = (xTuDongMo - 2 * x2) / 2, y3 = (yTuDongMo - 2 * y1), z3 = dayTu1;
	ctm = Translate(x3 * 0.5, y3 * 0.5, -z3 * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	setColor(colorPlasticWhite);
	// trụ tay nắm cửa tủ bên trái
	GLfloat x4 = dayTayNamTu, y4 = dayTayNamTu, z4 = 0.015;
	ctm = Translate(x3 - 0.1, y3 * 0.5, z4 * 0.5) * Scale(x4, y4, z4);
	createElement(ctm);

	// tay nắm cửa tủ bên trái
	GLfloat x5 = duongKinhTayNam / (sqrt(2)), y5 = x5, z5 = 0.005;
	for (int i = 0; i < 90; i++) {
		ctm = Translate(x3 - 0.1, y3 * 0.5, z4 + 0.5 * z5) * RotateZ(i) * Scale(x5, y5, z5);
		createElement(ctm);
	}

	mdl = modelStack.pop();

	// cửa phải tủ đóng mở
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(2 * x3 + dayTu1, dayTu1, 0) * RotateY(openCabinet);
	ctm = Translate(-x3 * 0.5, y3 * 0.5, -z3 * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	setColor(colorPlasticWhite);
	// trụ tay nắm cửa tủ bên phải
	ctm = Translate(-x3 + 0.1, y3 * 0.5, z4 * 0.5) * Scale(x4, y4, z4);
	createElement(ctm);

	// tay nắm cửa tủ bên trái
	for (int i = 0; i < 90; i++) {
		ctm = Translate(-x3 + 0.1, y3 * 0.5, z4 + 0.5 * z5) * RotateZ(i) * Scale(x5, y5, z5);
		createElement(ctm);
	}

	mdl = modelStack.pop();

	mdl = modelStack.pop();
}

void tuTivi(GLfloat tX, GLfloat tY, GLfloat tZ) {
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	tuDongMo(0, 0, 0);
	tuKeoNho(0, yTuDongMo, keoTu);
	tuKeoNho(xTuKeoNho, yTuDongMo, keoTu * 0.8);
	tuKeoTo(xTuDongMo, 0, keoTu);
	tuKeoTo(xTuDongMo, yTuKeoTo, keoTu * 0.85);
	tuKeoTo(xTuDongMo, 2 * yTuKeoTo, keoTu * 0.7);
	tuDongMo(xTuDongMo + xTuKeoTo, 0, 0);
	tuKeoNho(xTuDongMo + xTuKeoTo, yTuDongMo, keoTu);
	tuKeoNho(xTuDongMo + xTuKeoTo + xTuKeoNho, yTuDongMo, keoTu * 0.8);

	mdl = modelStack.pop();
}

/// <summary>
/// VẼ TI VI
/// </summary>

GLfloat xTivi = 1.4, yTivi = 0.85, zTivi = 0.018;

void tivi(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorPlasticBlack);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	// viền trên màn hình tivi
	GLfloat x1 = xTivi, y1 = dayVienTivi, z1 = dayVienTivi;
	ctm = Translate(x1 * 0.5, yTivi - y1 * 0.5, -z1 * 0.5) * Scale(x1, y1, z1);
	createElement(ctm);

	// viền dưới màn hình tivi
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z1 * 0.5) * Scale(x1, y1, z1);
	createElement(ctm);

	// viền trái màn hình tivi
	GLfloat x2 = dayVienTivi, y2 = yTivi, z2 = dayVienTivi;
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z2 * 0.5) * Scale(x2, y2, z2);
	createElement(ctm);

	// viền phải màn hình tivi
	ctm = Translate(xTivi - x2 * 0.5, y2 * 0.5, -z2 * 0.5) * Scale(x2, y2, z2);
	createElement(ctm);

	// mặt sau ti vi
	GLfloat x3 = xTivi, y3 = yTivi, z3 = zTivi - dayVienTivi;
	ctm = Translate(x3 * 0.5, y3 * 0.5, -z3 * 0.5 - z1) * Scale(x3, y3, z3);
	createElement(ctm);

	setColor(colorScreen);
	// màn hình tivi
	GLfloat x4 = xTivi - 2 * x2, y4 = yTivi - 2 * y1, z4 = z1;
	ctm = Translate(x4 * 0.5 + x2, y4 * 0.5 + y1, -z4 * 0.5) * Scale(x4, y4, z4);
	createElement(ctm);

	mdl = modelStack.pop();
}

/// <summary>
/// VẼ GHẾ SOFA
/// </summary>

/// định nghĩa thông số
// sofa đơn
GLfloat xSofaDon = 0.5, ySofaDon = 0.4, zSofaDon = 0.5;
// tựa ghế sofa
GLfloat xTuaSofa = xSofaDon, yTuaSofa = 0.3, zTuaSofa = 0.1;

void chanGheSofa(GLfloat tX, GLfloat  tY, GLfloat  tZ) {
	GLfloat x = dayChanGheSofa, y = x, z = x;

	ctm = Translate(x * 0.5 + tX, y * 0.5 + tY, -z * 0.5 + tZ) * Scale(x, y, z);
	createElement(ctm);
}
void tuaGheSofa(GLfloat tX, GLfloat  tY, GLfloat  tZ) {
	GLfloat x = xTuaSofa, y = yTuaSofa, z = zTuaSofa;
	ctm = Translate(x * 0.5 + tX, y * 0.5 + tY, -z * 0.5 + tZ) * Scale(x, y, z);
	createElement(ctm);
}

void sofaDon(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorSofa);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	GLfloat x = xSofaDon, y = ySofaDon, z = zSofaDon;
	ctm = Translate(x * 0.5, y * 0.5 + dayChanGheSofa, -z * 0.5) * Scale(x, y, z);
	createElement(ctm);

	setColor(colorWood);
	chanGheSofa(0, 0, 0);
	chanGheSofa(x - dayChanGheSofa, 0, 0);
	chanGheSofa(0, 0, -z + dayChanGheSofa);
	chanGheSofa(x - dayChanGheSofa, 0, -z + dayChanGheSofa);

	setColor(colorSofa);
	tuaGheSofa(0, dayChanGheSofa + ySofaDon, -zSofaDon + zTuaSofa);

	mdl = modelStack.pop();
}

void gheSofa(GLfloat tX, GLfloat tY, GLfloat tZ) {
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ - xSofaDon);
	sofaDon(0, 0, 0);
	sofaDon(xSofaDon, 0, 0);
	sofaDon(2 * xSofaDon, 0, 0);

	modelStack.push(mdl);
	mdl *= Translate(2 * xSofaDon, 0, 0) * RotateY(-90);
	sofaDon(0, 0, 0);
	mdl = modelStack.pop();

	modelStack.push(mdl);
	mdl *= Translate(3 * xSofaDon, dayChanGheSofa + ySofaDon, 0) * RotateY(90);
	tuaGheSofa(0, 0, 0);
	mdl = modelStack.pop();

	mdl = modelStack.pop();
}

/// <summary>
/// VẼ BÀN PHÒNG KHÁCH
/// </summary>

// định nghĩa thông số
GLfloat keoTuBan = 0; // điều khiển kéo mở tủ bàn

// thân bàn
GLfloat xBanPK = 0.9, yBanPK = 0.6, zBanPK = 0.4;
// tủ bàn
GLfloat canhBan = 0.05, xTuBan = xBanPK - 2 * canhBan, yTuBan = 0.18, zTuBan = zBanPK;

// tủ bàn

void tuBan(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorPlasticWhite);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	// cạnh trái tủ bàn
	GLfloat x1 = dayTu1, y1 = yTuBan, z = zTuBan;
	ctm = Translate(x1 * 0.5, y1 * 0.5, -zTuBan * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh phải tủ bàn
	ctm = Translate(xTuBan - x1 * 0.5, y1 * 0.5, -zTuBan * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh trước tủ bàn
	GLfloat x2 = xTuBan, y2 = yTuBan, z2 = dayTu1;
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z2 * 0.5) * Scale(x2, y2, z2);
	createElement(ctm);

	// cạnh sau tủ bàn
	ctm = Translate(x2 * 0.5, y2 * 0.5, -zTuBan + z2 * 0.5) * Scale(x2, y2, z2);
	createElement(ctm);

	// cạnh đáy tủ bàn
	setColor(colorPlasticBlack);
	GLfloat x3 = xTuBan - 2 * dayTu1, y3 = dayTu1, z3 = zTuBan - 2 * dayTu1;
	ctm = Translate(x3 * 0.5 + x1, y3 * 0.5, -z3 * 0.5 - z2) * Scale(x3, y3, z3);
	createElement(ctm);

	mdl = modelStack.pop();
}

void banPK(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorPlasticWhite);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	// cạnh trái bàn phòng khách
	GLfloat x1 = canhBan, y1 = yBanPK, z = zBanPK;
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh phải bàn phòng khách
	ctm = Translate(xBanPK - x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh đáy tủ
	GLfloat x2 = xBanPK - 2 * canhBan, y2 = (yBanPK - 0.05 - 0.2 - yTuBan) / 2;
	ctm = Translate(x2 * 0.5 + x1, y2 * 0.5 + 0.05, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// cạnh giữa tủ
	ctm = Translate(x2 * 0.5 + x1, y2 * 0.5 + 0.05 + y2 + yTuBan, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	setColor(colorGlass);
	GLfloat x3 = x2, y3 = dayKinh1;
	ctm = Translate(x3 * 0.5 + x1, yBanPK - y3 * 0.5, -z * 0.5) * Scale(x3, y3, z);
	createElement(ctm);

	tuBan(x1, 0.05 + y2, keoTuBan);

	mdl = modelStack.pop();
}

void createSphere(mat4 ins) {
	modelStack.push(mdl);
	mdl *= ins;
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, mdl);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glutSolidSphere(0.5, 64, 64);
	glutSolidCylinder(0.5, 0.5, 32, 32);
	mdl = modelStack.pop();

}

/// <summary>
/// VẼ ĐỒNG HỒ
/// </summary>


GLfloat xDongHo = 0.4, yDongHo = 0.4, zDongHo = 0.01;


void dongHo(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	GLfloat x1 = xDongHo / 2, y1 = yDongHo, z1 = zDongHo;
	for (int i = 0; i < 4; i++) {
		ctm = Translate(x1 * 0.5, yDongHo * 0.5, -z1 * 0.5) * RotateZ(i * 45) * Scale(x1, y1, z1);
		createElement(ctm);
	}
	mdl = modelStack.pop();
}

/// <summary>
/// VẼ TRANH
/// </summary>
GLfloat xTranh = 1.2, yTranh = 0.6, zTranh = 0.015;

void tranh(GLfloat tX, GLfloat tY, GLfloat tZ) {
	setColor(colorWood);
	modelStack.push(mdl);
	mdl *= Translate(tX, tY, tZ);

	// cạnh trên khung tranh
	GLfloat x1 = xTranh, y1 = dayKhungCua, z = dayKhungCua;
	ctm = Translate(x1 * 0.5, yTranh - y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh dưới khung tranh 
	ctm = Translate(x1 * 0.5, y1 * 0.5, -z * 0.5) * Scale(x1, y1, z);
	createElement(ctm);

	// cạnh trái khung tranh
	GLfloat x2 = dayKhungCua, y2 = yTranh;
	ctm = Translate(x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// cạnh phải khung tranh
	ctm = Translate(xTranh - x2 * 0.5, y2 * 0.5, -z * 0.5) * Scale(x2, y2, z);
	createElement(ctm);

	// phần dưới tranh
	setColor(colorGlass);
	GLfloat x3 = xTranh - 2 * dayKhungCua, y3 = 0.15, z3 = 0.01;
	ctm = Translate(x3 * 0.5 + x2, y3 * 0.5 + y1, -z * 0.5) * Scale(x3, y3, z3);
	createElement(ctm);

	// phần trên tranh
	setColor(colorGrass);
	GLfloat x4 = xTranh - 2 * dayKhungCua, y4 = yTranh - 2 * dayKhungCua - 0.15;
	ctm = Translate(x3 * 0.5 + x2, y4 * 0.5 + y1 + 0.15, -z * 0.5) * Scale(x4, y4, z3);
	createElement(ctm);

	mdl = modelStack.pop();
}

/// <summary>
/// PHÒNG KHÁCH
/// </summary>

void phongKhach(GLfloat tX, GLfloat tY, GLfloat tZ) {
	modelStack.push(mdl);
	mdl *= model_view;

	tuongPhongKhach(0, 0, 0);

	cuaTruoc((xTuongTruoc - xCuaTruoc) / 2, 0, 0);

	modelStack.push(mdl);
	mdl *= Translate(xSan, cuaCachDat + dayKhungCua, -(zTuongPhai - zCuaSoTP) * 0.5) * RotateY(90);
	cuaSo(0, 0, 0);
	mdl = modelStack.pop();

	tuTivi(xSan - dayTuong - xBac + dayBac, dayBac, -zSan + 1 + zBac);
	tivi(xSan - dayTuong - xBac / 2 - xTivi / 2, yBac + 0.1, -zSan + 1 + zTivi);

	modelStack.push(mdl);
	mdl *= Translate(xSan - dayTuong - zSofaDon - xSofaDon, 0, -3 * xSofaDon - 1) * RotateY(-90);
	gheSofa(0, 0, 0);
	mdl = modelStack.pop();

	modelStack.push(mdl);
	mdl *= Translate(xSan - dayTuong - zSofaDon - zBanPK - 0.1, 0, -xBanPK - 1 - xSofaDon - 0.1) * RotateY(-90);
	banPK(0, 0, 0);
	mdl = modelStack.pop();


	modelStack.push(mdl);
	mdl *= Translate(dayTuong + zDongHo, 1.5, -1.8) * RotateY(90);
	dongHo(0, 0, 0);
	mdl = modelStack.pop();

	modelStack.push(mdl);
	mdl *= Translate(dayTuong + zTranh + 0.03, 1.5, -0.3) * RotateY(90);
	tranh(0, 0, 0);
	mdl = modelStack.pop();

	mdl = modelStack.pop();
}


// Phát End

bool checkGocTren = true;

void display(void)
{
	projection_view();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const vec3 viewer_pos(0.0, 0.0, 0.0);  /*Trùng với eye của camera*/
	TDuong = Translate(0, 0, -3.65);
	model_view = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);
	if (checkGocTren) {
		tranNha();
	}

	phongNgu();
	phongNgu1();
	NhaBep();
	phongKhach(0, 0, 0);
	glutSwapBuffers();
}
void spinCube(void)
{
	quayCanhQuat += 10;
	if (quayCanhQuat > 360.0) {
		quayCanhQuat -= 360.0;
	}
	if (runQuat == 1) {
		quayQuat += 10;
	}
	else if (runQuat == 2) {
		quayQuat += 40;
	}
	else {
		quayQuat = 0;
	}
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler
	switch (key) {
		case '0': {
			xeye = 0, yeye = 4, zeye = 4;
			xat = 0, yat = 2, zat = -3.5;
			xup = 0, yup = 1, zup = 0;
		}
		case '7': {
			theta[0] += 5;
			break;
		}
		case '8': {
			theta[1] -= 5;
			break;
		}
		case '9': {
			theta[2] += 5;
			break;
		}
		
		


				//Key phòng ngủ
		case 'q': {
			moCua -= 5;
			quayTN -= 5;
			if (moCua < -85) {
				moCua = -85;
				quayTN = -90;
			}
			glutPostRedisplay();
			break;
		}
		case 'Q': {
			moCua += 5;
			quayTN += 5;
			if (moCua > 0) {
				moCua = 0;
				quayTN = 0;
			}
			glutPostRedisplay();
			break;
		}
		case 'w': {
			keoP += 0.1;
			keoCTP -= 0.1;
			if (keoP > xCanhCuaSo)	keoP = xCanhCuaSo;
			if (keoCTP < -rongTuQA / 2)	keoCTP = -rongTuQA / 2;
			glutPostRedisplay();
			break;
		}
		case 'W': {
			keoP -= 0.1;
			keoCTP += 0.1;
			if (keoP < 0)	keoP = 0;
			if (keoCTP > 0)	keoCTP = 0;
			glutPostRedisplay();
			break;
		}
		case 'e': {
			keoT -= 0.1;
			keoCTT += 0.1;
			if (keoT < -xCanhCuaSo)	keoT = -xCanhCuaSo;
			if (keoCTT > rongTuQA / 2)	keoCTT = rongTuQA / 2;
			glutPostRedisplay();
			break;
		}
		case 'E': {
			keoT += 0.1;
			keoCTT -= 0.1;
			if (keoT > 0)	keoT = 0;
			if (keoCTT < 0)	keoCTT = 0;
			glutPostRedisplay();
			break;
		}
				// Điều khiển ngăn kéo bàn 
		case 't': {
			l += 0.1;
			if (l > 0.4)	l = 0.4;
			glutPostRedisplay();
			break;
		}
		case 'T': {
			l -= 0.1;
			if (l < 0)	l = 0;
			glutPostRedisplay();
			break;
		}
		case 'r': {
			r += 0.1;
			if (r > 0.4)	r = 0.4;
			glutPostRedisplay();
			break;
		}
		case 'R': {
			r -= 0.1;
			if (r < 0)	r = 0;
			glutPostRedisplay();
			break;
		}
		case 'y': {
			moCuaTuT -= 5;
			if (moCuaTuT < -100)	moCuaTuT = -100;
			glutPostRedisplay();
			break;
		}
		case 'Y': {
			moCuaTuT += 5;
			if (moCuaTuT > 0)	moCuaTuT = 0;
			glutPostRedisplay();
			break;
		}
		case 'u': {
			moCuaTuP += 5;
			if (moCuaTuP > 100)	moCuaTuP = 100;
			glutPostRedisplay();
			break;
		}
		case 'U': {
			moCuaTuP -= 5;
			if (moCuaTuP < 0)	moCuaTuP = 0;
			glutPostRedisplay();
			break;
		}
		case 'i': {
			moCuaTuQAT -= 5;
			if (moCuaTuQAT < -90)	moCuaTuQAT = -90;
			glutPostRedisplay();
			break;
		}
		case 'I': {
			moCuaTuQAT += 5;
			if (moCuaTuQAT > 0)	moCuaTuQAT = 0;
			glutPostRedisplay();
			break;
		}
		case 'o': {
			moCuaTuQAP += 5;
			if (moCuaTuQAP > 100)	moCuaTuQAP = 100;
			glutPostRedisplay();
			break;
		}
		case 'O': {
			moCuaTuQAP -= 5;
			if (moCuaTuQAP < 0)	moCuaTuQAP = 0;
			glutPostRedisplay();
			break;
		}
		case 'p': {
			keoGhe += 0.1;
			if (keoGhe > 1)	keoGhe = 1;
			glutPostRedisplay();
			break;
		}
		case 'P': {
			keoGhe -= 0.1;
			if (keoGhe < 0)	keoGhe = 0;
			glutPostRedisplay();
			break;
		}


				//Key phòng bếp
		case '!': {
			if (on) {
				bd = 0;	
				on = false;
			}
			else {
				bd = 1;
				on = true;
			}
			break;
		}

		case 'a': {   // mở cửa sổ bếp 1
			if (!checkKeo) {
				keoCuaSo1 += 0.01;
				if (keoCuaSo1 >= (daiCuaSo - dayKhungCuaSo * 2) / 2) {
					keoCuaSo1 = (daiCuaSo - dayKhungCuaSo * 2) / 2;
					checkKeo = true;
				}
			}
			else {
				keoCuaSo1 -= 0.01;
				if (keoCuaSo1 <= 0) {
					keoCuaSo1 = 0;
					checkKeo = false;
				}
			}
			break;
		}
		case 'A': { // mở cửa sổ bếp 2
			if (!checkKeo) {
				keoCuaSo2 += 0.01;
				if (keoCuaSo2 >= (daiCuaSo - dayKhungCuaSo * 2) / 2) {
					keoCuaSo2 = (daiCuaSo - dayKhungCuaSo * 2) / 2;
					checkKeo = true;
				}
			}
			else {
				keoCuaSo2 -= 0.01;
				if (keoCuaSo2 <= 0) {
					keoCuaSo2 = 0;
					checkKeo = false;
				}
			}
			break;
		}
		case 's': {  // mở cửa sau 
			if (!checkMoCuaBepSau) {
				moCuaBepSau += 5;
				if (moCuaBepSau >= 90) {
					moCuaBepSau = 90;
					checkMoCuaBepSau = true;
				}
			}
			else {
				moCuaBepSau -= 5;
				if (moCuaBepSau <= 0) {
					moCuaBepSau = 0;
					checkMoCuaBepSau = false;
				}
			}
			break;
		}
		case 'd': {  // mở cửa tủ lạnh ngăn đông
			if (!checkMoTuLanh) {
				moTuNganDong += 5;
				if (moTuNganDong >= 90) {
					moTuNganDong = 90;
					checkMoTuLanh = true;
				}
			}
			else {
				moTuNganDong -= 5;
				if (moTuNganDong <= 0) {
					moTuNganDong = 0;
					checkMoTuLanh = false;
				}
			}
			break;
		}
		case 'D': {   // mở cửa tủ lạnh ngăn lạnh
			if (!checkMoTuLanh) {
				moTuNganLanh += 5;
				if (moTuNganLanh >= 90) {
					moTuNganLanh = 90;
					checkMoTuLanh = true;
				}
			}
			else {
				moTuNganLanh -= 5;
				if (moTuNganLanh <= 0) {
					moTuNganLanh = 0;
					checkMoTuLanh = false;
				}
			}
			break;
		}
		case 'f': {  // Kéo ghế ngồi ăn
			if (!checkKeoGheNgoi) {
				keoGheNgoi += 0.01;
				if (keoGheNgoi >= 0.2) {
					keoGheNgoi = 0.2;
					checkKeoGheNgoi = true;
				}
			}
			else {
				keoGheNgoi -= 0.01;
				if (keoGheNgoi <= 0) {
					keoGheNgoi = 0;
					checkKeoGheNgoi = false;
				}
			}
			break;
		}
		case 'F': {  //quay bàn ăn
			quayBanAn += 5;
			break;
		}
		case 'g': {  //Tủ
			if (!checkMoTuTreo) {
				moTuTreo += 5;
				if (moTuTreo >= 90) {
					moTuTreo = 90;
					checkMoTuTreo = true;
				}
			}
			else {
				moTuTreo -= 5;
				if (moTuTreo <= 0) {
					moTuTreo = 0;
					checkMoTuTreo = false;
				}
			}
			break;
		}
		case 'h': {
			if (!checkBatBepGa) {
				batBepGa += 10;
				if (batBepGa >= 90) {
					batBepGa = 90;
					checkBatBepGa = true;
				}
			}
			else {
				batBepGa -= 10;
				if (batBepGa <= 0) {
					batBepGa = 0;
					checkBatBepGa = false;
				}
			}

			break;
		}
		case 'j': {
			runQuat = 0;
			break;
		}
		case 'J': {  // quay đầu quạt
			if (!checkQuayDauQuat) {
				quayDauQuat += 5;
				if (quayDauQuat >= 90) {
					quayDauQuat = 90;
					checkQuayDauQuat = true;
				}
			}
			else {
				quayDauQuat -= 5;
				if (quayDauQuat <= -90) {
					quayDauQuat = -90;
					checkQuayDauQuat = false;
				}
			}
		}
		case 'k': { // nâng quạt lên
			nangQuat += 0.2;
			if (nangQuat >= hThan / 2) {
				nangQuat = hThan / 2;
			}
			break;
		}
		case 'K': {// hạ quạt xuống
			nangQuat -= 0.2;
			if (nangQuat <= 0) {
				nangQuat = 0;
			}
			break;
		}

				// Key phòng khách

		case 'c':
			openDoor += 5;
			if (openDoor > 90) openDoor = 90;
			break;
		case 'C':
			openDoor -= 5;
			if (openDoor < 0) openDoor = 0;
			break;
		case 'v':
			openWindow += 5 * 0.02;
			if (openWindow > (xCanhCS - 2 * dayCanhCS)) openWindow = xCanhCS - 2 * dayCanhCS;
			break;
		case 'V':
			openWindow -= 5 * 0.02;
			if (openWindow < 0) openWindow = 0;
			break;
		case 'b':
			openCabinet += 5;
			if (openCabinet > 90) openCabinet = 90;
			break;
		case 'B':
			openCabinet -= 5;
			if (openCabinet < 0) openCabinet = 0;
			break;
		case 'n':
			keoTu += 0.05;
			if (keoTu > zBac * 0.9) keoTu = zBac * 0.9;
			break;
		case 'N':
			keoTu -= 0.05;
			if (keoTu < 0) keoTu = 0;
			break;
		case 'm':
			keoTuBan += 0.02;
			if (keoTuBan > zTuBan * 0.9) keoTuBan = zTuBan * 0.85;
			break;
		case 'M':
			keoTuBan -= 0.02;
			if (keoTuBan < 0) keoTuBan = 0;
			break; 
		case '[': {
			colorScreen = colorGlass;
			glutPostRedisplay();
			break;
		} 
		case ']': {
			colorScreen = colorScreenoff;
			glutPostRedisplay();
			break;
		}
		
			// Bao quát trên cao
		case '1': {
			xeye = 0, yeye = 5, zeye = -3.5;
			xat = 0, yat = 0, zat = -3.5;
			xup = -1, yup = 0, zup = 0;
			checkGocTren = false;
			break;
		}
				// Nhìn phòng khách
		case '2': {
			xeye = 1.5, yeye = 2.5, zeye = -0.5;
			xat = 1.5, yat = 1.1, zat = -3.5;
			xup = 0, yup = 1, zup = 0;
			checkGocTren = true;
			break;
		}
				// Nhìn phòng ngủ góc 1
		case '3': {
			xeye = -1.5, yeye = 2.5, zeye = -0.7;
			xat = -1.5, yat = 0.8, zat = -3.5;
			xup = 0, yup = 1, zup = 0;
			checkGocTren = true;
			break;
		}
				// Nhìn phòng ngủ góc 2
		case '4': {
			xeye = -1.2, yeye = 2.5, zeye = -2.5;
			xat = -1.5, yat = 0.8, zat = 0;
			xup = 0, yup = 1, zup = 0;
			checkGocTren = true;
			break;
		}
				// Nhìn phòng bếp
		case '5': {
			xeye = 1.5, yeye = 2.5, zeye = -4.5;
			xat = 1.5, yat = 0.8, zat = -6.75;
			xup = 0, yup = 1, zup = 0;
			checkGocTren = true;
			break;
		}
		case '6': {
			xeye = 0, yeye = 5, zeye = 2;
			xat = 0, yat = 2, zat = -3.5;
			xup = 0, yup = 1, zup = 0;
			checkGocTren = true;
			break;
		} 
		case ',': {
			xeye += 0.05;
			checkGocTren = true;
			break;
		}
		case '<': {
			xeye -= 0.05;
			checkGocTren = true;
			break;
		}
		case '.': {
			yeye += 0.05;
			checkGocTren = true;
			break;
		}
		case '>': {
			yeye -= 0.05;
			checkGocTren = true;
			break;
		}
		case '/': {
			zeye += 0.05;
			checkGocTren = true;
			break;
		}
		case '?': {
			zeye -= 0.05;
			checkGocTren = true;
			break;
		}
		


	}
	glutPostRedisplay();
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

	glutIdleFunc(spinCube);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
