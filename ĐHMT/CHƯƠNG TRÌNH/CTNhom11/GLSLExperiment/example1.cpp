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

GLuint program;
GLuint model_view_loc;
GLuint loc_projection; 
GLuint view_loc;

// Các tham số cho viewing
GLfloat l = -0.8, r = 0.8, t = 1.0, b = -1.0, zNear = 0.8, zFar = 3.8;
GLfloat radius = 1, thetal = 0, phi = 0;
GLfloat dr = 5 * M_PI / 180;
GLfloat theta[3] = { 0,0,0 };

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
	/*colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;*/
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


void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );     
    glBindVertexArray( vao );

    // Tạo và khởi tạo một buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    //glBufferData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW );
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

color4 material_ambient(1.0, 0.0, 1.0, 1.0);
color4 material_diffuse;
color4 material_specular;
float material_shininess = 100.0;

color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product;
color4 specular_product = light_specular * material_specular;



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

	
	
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	model_view_loc = glGetUniformLocation(program, "model_view");
	loc_projection = glGetUniformLocation(program, "projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

mat4 ins;
mat4 quayBase;

vec4 mauDen = vec4(0.0, 0.0, 0.0, 1.0);
vec4 mauTrang = vec4(1.0, 1.0, 1.0, 1.0);
vec4 mauDo = vec4(1.0, 0.0, 0.0, 1.0);
vec4 mauTim = vec4(1.0, 0.0, 1.0, 1.0);
vec4 mauVang = vec4(1.0, 1.0, 0.0, 1.0);
vec4 mauLuc = vec4(0.0, 1.0, 0.0, 1.0);
vec4 mauXanhLam = vec4(0.0, 1.0, 1.0, 1.0);
vec4 mauXanhLo = vec4(0.0, 0.0, 1.0, 1.0);

void matPhang(GLfloat x, GLfloat y, GLfloat z, mat4 mt, vec4 colorCode) {

	material_diffuse = colorCode;
	material_specular = colorCode;
	diffuse_product = light_diffuse * material_diffuse;

	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	
	vec4 eye(0.0, 0.0, 2.0, 1.0);
	vec4 at(0.0, 0.0, 1.0, 1.0);
	vec4 up(0.0, 1.0, 0.0, 1.0);
	mat4 mv = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, mv);
	mat4 p = Frustum(l, r, b, t, zNear, zFar);
	glUniformMatrix4fv(loc_projection, 1, GL_TRUE, p);

	mat4 mp = Scale(x, y, z);
	glUniformMatrix4fv(model_view_loc,1 , GL_TRUE, mv * quayBase * mt * mp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

GLfloat sanx = 8, sany = 0.05, sanz = 8;
void san() {
	ins = Translate(0, -0.025, 0);
	matPhang(sanx, sany, sanz, ins, mauVang);
}

GLfloat tuong1x = 0.05, tuong1y = 3, tuong1z = 3;
GLfloat tuong2x = 3 + 0.1, tuong2y = 3, tuong2z = 0.05;
GLfloat tuong3x = 3 + 0.1, tuong3y = 0.05, tuong3z = 3 + 0.05;
void _3mattuong() {
	//tuong1.1
	ins = Translate(-1.5 - 0.025, 1.5, 0);
	matPhang(tuong1x, tuong1y, tuong1z, ins, mauLuc);
	//tuong1.2
	ins = Translate(1.5 + 0.025, 1.5, 0);
	matPhang(tuong1x, tuong1y, tuong1z, ins, mauLuc);
	//tuong2
	ins = Translate(0, 1.5, -1.5 - 0.025);
	matPhang(tuong2x, tuong2y, tuong2z, ins, mauLuc);
	//tuong3
	ins = Translate(0, 0.025 + 3, -0.025);
	matPhang(tuong3x, tuong3y, tuong3z, ins, mauLuc);
}

GLfloat tdx = 0.05, tdy = 1.55, tdz = 0.05;
GLfloat tnx = 3, tny = 0.05, tnz = 1;
void ke() {
	//thanh doc dưới trái
	ins = Translate(-1.5 + 0.025, tdy / 2, -1.5 + 0.025);
	matPhang(tdx, tdy, tdz, ins, mauDen);
	//thanh doc dưới phải
	ins = Translate(1.5 - 0.025, tdy / 2, -1.5 + 0.025);
	matPhang(tdx, tdy, tdz, ins, mauDen);
	//thanh doc dưới giữa
	ins = Translate(0, tdy / 2, -1.5 + 0.025);
	matPhang(tdx, tdy, tdz, ins, mauDen);

	//thanh doc trên trái
	ins = Translate(-1.5 + 0.025, tdy / 2, -0.5 - 0.025);
	matPhang(tdx, tdy, tdz, ins, mauDen);
	//thanh doc trên phải
	ins = Translate(1.5 - 0.025, tdy / 2, -0.5 - 0.025);
	matPhang(tdx, tdy, tdz, ins, mauDen);
	//thanh doc trên giữa
	ins = Translate(0, tdy / 2, -0.5 - 0.025);
	matPhang(tdx, tdy, tdz, ins, mauDen);

	//thanh ngang mái
	ins = Translate(0, tny / 2 + 1.5, -1.5 + tnz / 2);
	matPhang(tnx, tny, tnz, ins, mauTrang);
	//thanh ngang trên
	ins = Translate(0, tny / 2 + 0.9, -1.5 + tnz / 2);
	matPhang(tnx, tny, tnz, ins, mauTrang);
	//thanh ngang dưới
	ins = Translate(0, tny / 2 + 0.3, -1.5 + tnz / 2);
	matPhang(tnx, tny, tnz, ins, mauTrang);
}

GLfloat mb1x = 0.05, mb1y = 0.75, mb1z = 0.75;
GLfloat mb2x = 0.05, mb2y = 0.25, mb2z = 0.2;

GLfloat mn1x = 1.2, mn1y = 0.05, mn1z = 0.2;
GLfloat mn2x = 1.2, mn2y = 0.05, mn2z = 0.75;

GLfloat mtx = 1.2, mty = 1.05, mtz = 0.05;

GLfloat mn3x = 0.5, mn3y = 0.05, mn3z = 0.75;

GLfloat mt2x = 0.5, mt2y = 0.7, mt2z = 0.02;
GLfloat qb =0;
void banthungan() {
	//mặt bên trái
	//mặt bên trái 1
	ins = Translate(-1.5 + mb1x / 2, mb1y / 2, 1.35 - mb1z / 2);
	matPhang(mb1x, mb1y, mb1z, ins, mauVang);
	//mặt bên trái 2
	ins = Translate(-1.5 + mb2x / 2, mb2y / 2 + mb1y, 1.35 - mb2z / 2);
	matPhang(mb2x, mb2y, mb2z, ins, mauVang);

	//mặt bên phải
	//mặt bên phải 1
	ins = Translate(-0.3 - mb1x / 2, mb1y / 2, 1.35 - mb1z / 2);
	matPhang(mb1x, mb1y, mb1z, ins, mauVang);
	//mặt bên phải 2
	ins = Translate(-0.3 - mb2x / 2, mb2y / 2 + mb1y, 1.35 - mb2z / 2);
	matPhang(mb2x, mb2y, mb2z, ins, mauVang);

	//thanh đỡ 2 mặt bàn
	ins = Translate(-0.9, mb2y / 2 + mb1y, 1.35 - mb2z / 2);
	matPhang(mb2x, mb2y, mb2z, ins, mauVang);

	//mat bàn trên
	ins = Translate(-1.5 + mn1x / 2, mn1y / 2 + 1, 1.35 - mn1z / 2);
	matPhang(mn1x, mn1y, mn1z, ins, mauVang);
	//mặt bàn dưới
	ins = Translate(-1.5 + mn2x / 2, mn2y / 2 + 0.75, 1.35 - mn2z / 2);
	matPhang(mn2x, mn2y, mn2z, ins, mauVang);

	//mặt trước
	ins = Translate(-1.5 + mtx / 2, mty / 2, 1.35 - mtz / 2);
	matPhang(mtx, mty, mtz, ins, mauVang);

	// ngắn kéo
	//mặt bên
	ins = Translate(-1 - mb1x / 2, mb1y / 2, 1.35 - mb1z / 2);
	matPhang(mb1x, mb1y, mb1z, ins, mauVang);
	//mặt dưới
	ins = Translate(-1.5 + mn3x / 2, mn3y / 2 + 0.05, 1.35 - mn3z / 2);
	matPhang(mn3x, mn3y, mn3z, ins, mauVang);
	//mặt trước
	ins = Translate(-1.5 , 0, 1.35 - 0.75) * RotateY(qb) * Translate(mt2x/2, mt2y / 2 + 0.05,-mt2z/2);
	matPhang(mt2x, mt2y, mt2z, ins, mauVang);
}

GLfloat cbx = 0.05, cby = 0.75, cbz = 0.05;
GLfloat mbx = 0.6, mby = 0.05, mbz = 1.2;
void bantauhoa() {
	//chân dưới phải
	ins = Translate(-cbx / 2 + 1.5, cby / 2, 0);
	matPhang(cbx, cby, cbz, ins, mauDen);
	//chân dưới trái
	ins = Translate(cbx / 2 + 1.5 - 0.6, cby / 2, 0);
	matPhang(cbx, cby, cbz, ins, mauDen);
	//chân trên phải
	ins = Translate(-cbx / 2 + 1.5, cby / 2, 1.2 - cbz);
	matPhang(cbx, cby, cbz, ins, mauDen);
	//chân trên trái
	ins = Translate(cbx / 2 + 1.5 - 0.6, cby / 2, 1.2 - cbz);
	matPhang(cbx, cby, cbz, ins, mauDen);
	//mat bàn
	ins = Translate(-mbx / 2 + 1.5, -mby / 2 + 0.75, mbz / 2 - cbz / 2);
	matPhang(mbx, mby, mbz, ins, mauTrang);
}

//biến dịch tàu
GLfloat ry = 180;
GLfloat tx = 1.2, ty = 1.02, tz = 0.9;

GLfloat muix = 0.2, muiy = 0.03, muiz = 0.03;
GLfloat mui1x = 0.03, mui1y = 0.3, mui1z = 0.03;
GLfloat mui2x = 0.03, mui2y = 0.28, mui2z = 0.03;
GLfloat mui3x = 0.03, mui3y = 0.2853, mui3z = 0.03;
GLfloat mui4x = 0.26378, mui4y = 0.03, mui4z = 0.03;
void muitauhoa() {

	//vẽ dầu tàu
	//đầu tàu
		//mũi tàu
	ins = Translate(tx, ty, tz) * RotateY(ry) * RotateX(35) * RotateZ(0) * Translate(0, -mui1y / 2, 0);
	matPhang(mui1x, mui1y, mui1z, ins, mauDo);

	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, 0, -0.02) * RotateY(-19) * RotateX(22) * Translate(0.05, -mui2y / 2, 0);
	matPhang(mui2x, mui2y, mui2z, ins, mauDo);
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, 0, -0.02) * RotateY(19) * RotateX(22) * Translate(-0.05, -mui2y / 2, 0);
	matPhang(mui2x, mui2y, mui2z, ins, mauDo);

	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, -0.015, 0) * RotateZ(22.14) * Translate(0.1, -mui3y / 2, 0);
	matPhang(mui3x, mui3y, mui3z, ins, mauDo);
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, -0.015, 0) * RotateZ(-22.14) * Translate(-0.1, -mui3y / 2, 0);
	matPhang(mui3x, mui3y, mui3z, ins, mauDo);


	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, 0, 0);
	matPhang(muix, muiy, muiz, ins, mauDo);
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(-0.2, 0, 0) * RotateY(40.695) * Translate(mui4x / 2, -0.24575, 0);
	matPhang(mui4x, mui4y, mui4z, ins, mauDo);
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0.2, 0, 0) * RotateY(-40.695) * Translate(-mui4x / 2, -0.24575, 0);
	matPhang(mui4x, mui4y, mui4z, ins, mauDo);
}

GLfloat taux = 0.4, tauy = 0.4, tauz = 0.3;
GLfloat tau2x = 0.4, tau2y = 0.4, tau2z = 0.4;
GLfloat conx = 0.2, cony = 0.03, conz = 0.05;

GLfloat ongx = 0.05, ongy = 0.1, ongz = 0.05;
GLfloat ong2x = 0.09, ong2y = 0.03, ong2z = 0.09;
void toatau() {
	//toa1
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, -0.05, tauz / 2 + muiz / 2);
	matPhang(taux, tauy, tauz, ins, mauTim);
	//nối toa 1-2
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, -mui1y / 2, conz / 2 + muiz / 2 + tauz);
	matPhang(conx, cony, conz, ins, mauTim);
	//toa2
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, -0.05, tau2z / 2 + muiz / 2 + tauz + conz);
	matPhang(tau2x, tau2y, tau2z, ins, mauTim);

	//ống khói
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, ongy / 2 + 0.15, ongz / 2 + 0.1);
	matPhang(ongx, ongy, ongz, ins, mauTrang);
	ins = Translate(tx, ty, tz) * RotateY(ry) * Translate(0, ong2y / 2 + 0.15 + ongy, ong2z / 4 + 0.1);
	matPhang(ong2x, ong2y, ong2z, ins, mauTrang);
}

void tauhoa() {
	muitauhoa();
	toatau();
}
GLfloat ccx = 3, ccy= 0.3, ccz= 0.15;
GLfloat cc1x = 3, cc1y = 0.3, cc1z = 0.03;

void cuacuon() {
	// hộp của cuốn
	ins = Translate(0,-ccy/2+3,-ccz/2+1.5);
	matPhang(ccx, ccy, ccz, ins, mauTrang);
	//canh của cuốn
	ins = Translate(0, -cc1y / 2 + 3, -cc1z / 2 + 1.5);
	matPhang(cc1x, cc1y, cc1z, ins, mauTrang);
}

void vemohinh() {
	//tau mo hinh 1
	ins = Translate(0.45, 0.5, -0.8);
	matPhang(0.7, 0.3, 0.2, ins,mauDo);
	ins = Translate(1.15, 0.5, -0.8);
	matPhang(0.5, 0.3, 0.2, ins, mauDo);
	ins = Translate(1.35, 0.75, -0.8);
	matPhang(0.1, 0.2, 0.1, ins, mauDo);
	//tau mo hinh 2
	ins = Translate(-0.45, 0.5, -0.8);
	matPhang(0.7, 0.3, 0.2, ins, mauTim);
	ins = Translate(-1.15, 0.5, -0.8);
	matPhang(0.5, 0.3, 0.2, ins, mauTim);
	ins = Translate(-1.3, 0.75, -0.8);
	matPhang(0.1, 0.2, 0.1, ins, mauTim);
	//tau mo hinmh 3
	ins = Translate(0.45, 1.1, -0.8);
	matPhang(0.7, 0.3, 0.2, ins, mauXanhLo);
	ins = Translate(1.15, 1.1, -0.8);
	matPhang(0.5, 0.3, 0.2, ins, mauXanhLo);
	ins = Translate(1.3, 1.3, -0.8);
	matPhang(0.1, 0.2, 0.1, ins, mauXanhLo);

	//tau mo hinh 4
	ins = Translate(-0.45, 1.1, -0.8);
	matPhang(0.7, 0.3, 0.2, ins, mauVang);
	ins = Translate(-1.15, 1.1, -0.8);
	matPhang(0.5, 0.3, 0.2, ins, mauVang);
	ins = Translate(-1.3, 1.3, -0.8);
	matPhang(0.1, 0.2, 0.1, ins, mauVang);
}



GLfloat dhx=1.5, dhy=0.5, dhz=0.2;
void DieuHoa() {
	ins = Translate(0, -dhy / 2 + 2.8 , dhz / 2 - 1.5);
	matPhang(dhx, dhy, dhz, ins, mauTrang);
}

void Base() {
	san();
	_3mattuong();
	ke();
	banthungan();
	tauhoa();
	bantauhoa();
	cuacuon();
	vemohinh();
	DieuHoa();
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );    
	const vec3 viewer_pos(0.0, 0.0, 1.9);
	quayBase = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);
	Base();
	
	glutSwapBuffers();									   
}


void keyboard( unsigned char key, int x, int y )
{

	switch (key) {
	case 033:  // Escape Key
	/*case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;*/
			
	case 'b': {
		if (qb < 85) {
			qb += 5;
			break;
		}
		glutPostRedisplay();
		break;
	}
	case 'B': {
		if (qb > 0) {
			qb -= 5;
			break;
		}
		glutPostRedisplay();
		break;
	}

	
			//case c : đóng mở cửa cuốn
	case 'c': {
		
		if (cc1y >= 0.3) {
			cc1y -= 0.1;
			break;
		}
		glutPostRedisplay();
		break;
	}
	case 'C': {

		if (cc1y <3) {
			cc1y += 0.1;
			break;
		}
		glutPostRedisplay();
		break;
	}

	case 'x': {
		theta[0] += 5;
		if (theta[0] > 360) {
			theta[0] = 0;
		}
		glutPostRedisplay();
		break;
	}

			//case x : quay gian hàng theo trục x
	case 'X': {
		theta[0] -= 5;
		if (theta[0] > 360) {
			theta[0] = 0;
		}
		glutPostRedisplay();
		break;
	}

			//case y : quay gian hàng theo trục y

	case 'y': {
		theta[1] += 5;
		if (theta[1] > 360) {
			theta[1] = 0;
		}
		glutPostRedisplay();
		break;
	}
	case 'Y': {
		theta[1] -= 5;
		if (theta[1] > 360) {
			theta[1] = 0;
		}
		glutPostRedisplay();
		break;
	}
			//case z : quay gian hàng theo trục z
	case 'z': {
		theta[2] += 5;
		if (theta[2] > 360) {
			theta[2] = 0;
		}
		glutPostRedisplay();
		break;
	}
	case 'Z': {
		theta[2] -= 5;
		if (theta[2] > 360) {
			theta[2] = 0;
		}
		glutPostRedisplay();
		break;
	}
	
	case '1': l *= 1.1; r *= 1.1; break; // thu nhỏ gian hàng theo chiều ngang
	case '2': l *= 0.9; r *= 0.9; break;// phóng to gian hàng theo chiều ngang
	case '3': b*= 1.1; t *= 1.1; break;// thu nhỏ gian hàng theo chiều dọc
	case '4': b *= 0.9; t *= 0.9; break;// phóng to gian hàng theo chiều dọc
	case '5': zNear *= 1.1; zFar *= 1.1; break;// nhìn gần gian hàng theo trục z
	case '6': zNear *= 0.9; zFar *= 0.9; break;// nhìn xa gian hàng theo trục z
	
	}
	glutPostRedisplay();
}




int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 1000, 1000 );                 
	glutInitWindowPosition(100,150);               
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
