#include <GL/glut.h>
#include <cmath>    // Untuk fungsi cos, sin, dan lainnya
#ifndef M_PI
#define M_PI 3.14159265358979323846 // Definisi konstanta PI jika belum ada
#endif

// Variabel untuk rotasi
float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
float scaleFactor = 1.0f;
// Variabel untuk mengatur translasi
float translateX = 0.0f;
float translateY = 0.0f;

float offsetZ = 0.0f; // Default offset on the Z-axis
float offsetY = 0.0f; // Default offset on the Y-axis


void init() {
    glEnable(GL_DEPTH_TEST); // Mengaktifkan pengujian kedalaman
    glEnable(GL_LIGHTING);   // Mengaktifkan pencahayaan
    glEnable(GL_LIGHT0);     // Mengaktifkan sumber cahaya 0
    glShadeModel(GL_FLAT);   // Menggunakan model bayangan flat (rata)

    // Parameter pencahayaan utama (cahaya titik) dengan intensitas yang lebih kecil
    GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Cahaya sekitar (lebih redup)
    GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.5f, 1.0f }; // Cahaya menyebar (lebih hangat, kuning)
    GLfloat light_specular[] = { 0.9f, 0.9f, 0.8f, 1.0f }; // Cahaya pantulan (lebih redup dari sebelumnya)
    GLfloat light_position[] = { -5.0f, -5.0f, -5.0f, 1.0f }; // Posisi cahaya lebih dekat dan di arah negatif

    // Mengatur parameter cahaya utama
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL); // Mengaktifkan material warna
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // Material warna

    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Warna biru langit
    glClear(GL_COLOR_BUFFER_BIT);            // Bersihkan layar dengan warna yang telah ditentukan
}

// Fungsi untuk menggambar lapangan
void drawLapangan(float x, float y, float z, float width, float depth) {
    glColor3f(0.3f, 0.7f, 0.3f); // Warna hijau
    glPushMatrix();
    glTranslatef(x, y - 0.1f, z); // Pastikan sedikit di bawah tugu
    glScalef(width, 0.1f, depth); // Skala sesuai ukuran tanah
    glutSolidCube(1.0f); // Menggambar tanah sebagai kubus besar
    glPopMatrix();
}

// Fungsi untuk menggambar tanah (ground)
void drawGround(float x, float y, float z, float width, float depth, float height) {
    // Warna dasar tanah
    glColor3f(0.0f, 0.0f, 0.0f); // Warna coklat tanah
    glPushMatrix();
    glTranslatef(x, y - height / 2.0f, z);  // Memastikan tanah berada di bawah gedung
    glScalef(width, height, depth);  // Skala sesuai ukuran tanah
    glutSolidCube(1.0f);  // Menggambar tanah sebagai kubus besar
    glPopMatrix();
}

// menggambar lingkaran dibawah base bertingkat
void drawPolygon(float centerX, float centerY, float centerZ, float radius, int sides) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * M_PI * i / sides; // Hitung sudut untuk tiap titik
        float x = centerX + radius * cos(angle);
        float z = centerZ + radius * sin(angle);
        glVertex3f(x, centerY, z); // Tambahkan titik ke dalam poligon
    }
    glEnd();
}

void drawBaseDecoration(float x, float y, float z, float radius) {
    glColor3f(0.5f, 0.5f, 0.5f); // Warna abu-abu
    drawPolygon(x, y, z, radius, 14); // Gambar segi 10 besar

    glColor3f(1.0f, 0.96f, 0.8f); // Warna krem
    drawPolygon(x, y + 0.01f, z, radius * 0.8f, 10); // Gambar segi 10 kecil di dalam


}

// Fungsi untuk menggambar detail emas
void drawGoldDetail(float x, float y, float z, float radius, float height) {
    glColor3f(1.0f, 0.84f, 0.0f); // Warna emas
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, radius, radius * 0.9f, height, 50, 50);
    glPopMatrix();
}

// Fungsi untuk menggambar puncak tugu
void drawTop(float x, float y, float z, float radius, float height) {
    glColor3f(1.0f, 0.84f, 0.0f); // Warna emas
    glPushMatrix();
    // Menurunkan posisi puncak lebih jauh dengan mengurangi nilai pada y
    glTranslatef(x, y - height * 0.1f, z);  // Menurunkan posisi puncak lebih jauh
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    float largerRadius = radius * 1.5f;  // Perbesar radius puncak
    float largerHeight = height * 1.5f;  // Perbesar tinggi puncak
    glutSolidCone(largerRadius, largerHeight, 50, 50);
    glPopMatrix();
}


// Fungsi untuk menggambar base bertingkat heksagonal
void drawBase(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Tingkat pertama base (warna hijau gelap)
    glColor3f(0.1f, 0.4f, 0.1f); // Warna hijau gelap
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(6.0f, 1.0f, 6.0f); // Skala untuk tingkat pertama
    glutSolidCube(1.0f);
    glPopMatrix();

    // Tingkat kedua base (warna hijau sedang)
    glColor3f(0.3f, 0.6f, 0.3f); // Warna hijau sedang
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(5.0f, 1.0f, 5.0f); // Skala untuk tingkat kedua
    glutSolidCube(1.0f);
    glPopMatrix();

    // Tingkat ketiga base (warna hijau terang)
    glColor3f(0.5f, 0.8f, 0.5f); // Warna hijau terang
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(4.0f, 1.0f, 4.0f); // Skala untuk tingkat ketiga
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

// Fungsi untuk menggambar base pada pilar
void drawPillarBase(float x, float y, float z, float size) {
    glColor3f(0.6f, 0.4f, 0.2f); // Warna coklat untuk base
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(size, 0.4f, size); // Base pendek dan lebar
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Fungsi untuk menggambar badan tugu
void drawBody(float x, float y, float z, float radius, float height) {
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, radius, radius * 0.5f, height, 50, 50);
    glPopMatrix();
}

// Fungsi untuk menggambar pilar
void drawPillar(float x, float y, float z, float radius, float height) {
    // Base pilar
    drawPillarBase(x, y, z, radius * 2.5f);

    // Pilar utama
    glColor3f(0.8f, 0.8f, 0.8f); // Warna abu-abu
    glPushMatrix();
    glTranslatef(x, y + 0.25f, z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, 50, 50); // Silinder utama pilar
    gluDeleteQuadric(quad);
    glPopMatrix();

    // Dekorasi bulat di atas pilar
    glColor3f(1.0f, 0.84f, 0.0f); // Warna emas
    glPushMatrix();
    glTranslatef(x, y + height + (radius * 0.5f), z); // Posisikan di atas pilar
    glutSolidSphere(radius * 1.2f, 50, 50); // Bola dekorasi
    glPopMatrix();
}


void drawTrapezoid3D(float base1, float base2, float height, float depth) {
    glBegin(GL_QUADS);
    // Depan
    glVertex3f(-base1 / 2, 0, depth / 2);
    glVertex3f(base1 / 2, 0, depth / 2);
    glVertex3f(base2 / 2, height, depth / 2);
    glVertex3f(-base2 / 2, height, depth / 2);

    // Belakang
    glVertex3f(-base1 / 2, 0, -depth / 2);
    glVertex3f(base1 / 2, 0, -depth / 2);
    glVertex3f(base2 / 2, height, -depth / 2);
    glVertex3f(-base2 / 2, height, -depth / 2);

    // Kiri
    glVertex3f(-base1 / 2, 0, depth / 2);
    glVertex3f(-base1 / 2, 0, -depth / 2);
    glVertex3f(-base2 / 2, height, -depth / 2);
    glVertex3f(-base2 / 2, height, depth / 2);

    // Kanan
    glVertex3f(base1 / 2, 0, depth / 2);
    glVertex3f(base1 / 2, 0, -depth / 2);
    glVertex3f(base2 / 2, height, -depth / 2);
    glVertex3f(base2 / 2, height, depth / 2);

    // Atas
    glVertex3f(-base2 / 2, height, depth / 2);
    glVertex3f(base2 / 2, height, depth / 2);
    glVertex3f(base2 / 2, height, -depth / 2);
    glVertex3f(-base2 / 2, height, -depth / 2);

    glEnd();
}

void drawTriangle(float base, float height) {
    glBegin(GL_TRIANGLES);
    glVertex3f(-base / 2, 0, 0);   // Kiri
    glVertex3f(base / 2, 0, 0);    // Kanan
    glVertex3f(0, height, 0);      // Atas
    glEnd();
}

void drawDetailedGround(float x, float y, float z, float width, float depth) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Pola lingkaran tengah (abu-abu)
    GLUquadric* quad = gluNewQuadric();
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0f, width * 0.2f, 32, 4);

    // Ring luar lingkaran
    gluCylinder(quad, width * 0.2f, width * 0.2f, 0.1f, 32, 2);
    glPopMatrix();

    // Area biru (kiri dan kanan)
    glColor3f(0.3f, 0.6f, 0.8f); // Warna biru kehijauan untuk pattern
    glBegin(GL_QUADS);
    // Area kiri
    glVertex3f(-width * 0.4f, 0.05f, -depth * 0.2f);
    glVertex3f(-width * 0.15f, 0.05f, -depth * 0.2f);
    glVertex3f(-width * 0.15f, 0.05f, depth * 0.2f);
    glVertex3f(-width * 0.4f, 0.05f, depth * 0.2f);

    // Area kanan
    glVertex3f(width * 0.15f, 0.05f, -depth * 0.2f);
    glVertex3f(width * 0.4f, 0.05f, -depth * 0.2f);
    glVertex3f(width * 0.4f, 0.05f, depth * 0.2f);
    glVertex3f(width * 0.15f, 0.05f, depth * 0.2f);
    glEnd();

    // Pattern dalam lingkaran (pola segi enam/radial)
    glColor3f(0.3f, 0.6f, 0.8f); // Warna biru kehijauan untuk pattern
    glPushMatrix();
    glTranslatef(0.0f, 0.11f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // Menonaktifkan pencahayaan sementara untuk menggambar objek tanpa bayangan
    glDisable(GL_LIGHTING);

    for (int i = 0; i < 8; i++) {
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f); // Rotasi untuk mendistribusikan segi enam dalam pola radial

        glBegin(GL_POLYGON);  // Mulai menggambar segi enam
        for (int j = 0; j < 6; j++) {
            float angle = 2.0f * M_PI * j / 6.0f;  // Menghitung sudut untuk tiap sisi segi enam
            glVertex3f(width * 0.19f * cos(angle), width * 0.19f * sin(angle), 0.0f); // Titik segi enam
        }
        glEnd();
    }

    // Mengembalikan pencahayaan
    glEnable(GL_LIGHTING);

    glPopMatrix();

    // Kotak taman kiri
    glColor3f(0.2f, 0.6f, 0.2f); // Hijau untuk kotak taman
    glPushMatrix();
    glTranslatef(-width * 0.275f, 0.1f, 0.0f);
    glScalef(width * 0.15f, 0.1f, depth * 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Kotak taman kanan
    glColor3f(0.2f, 0.6f, 0.2f); // Hijau untuk kotak taman
    glPushMatrix();
    glTranslatef(width * 0.275f, 0.1f, 0.0f);
    glScalef(width * 0.15f, 0.1f, depth * 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    gluDeleteQuadric(quad);
    glPopMatrix();
}

// Fungsi untuk menggambar pohon dengan offset pada sumbu X
void drawTree(float x, float y, float z, float offsetX) {
    // Batang pohon
    glColor3f(0.55f, 0.27f, 0.07f); // Warna coklat
    glPushMatrix();
    // Geser batang pohon pada sumbu X, Y, dan Z
    glTranslatef(x + offsetX, y + offsetY, z + offsetZ);  // Geser sesuai offset
    // Rotasi batang pohon untuk membentuk posisi tegak
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // Rotasi agar batang berdiri tegak
    GLUquadric* trunk = gluNewQuadric();
    gluCylinder(trunk, 0.3f, 0.3f, 3.0f, 20, 20); // Menggambar batang pohon
    gluDeleteQuadric(trunk);
    glPopMatrix();

    // Daun pohon
    glColor3f(0.0f, 0.8f, 0.0f); // Warna hijau
    glPushMatrix();
    glTranslatef(x + offsetX, y + 3.0f, z); // Geser posisi daun sesuai offset
    glutSolidSphere(1.5f, 20, 20); // Lingkaran daun pohon
    glPopMatrix();
}

// Fungsi utama untuk menggambar tugu
void drawMonument(float x, float y, float z) {
    float baseHeight = 2.0f;
    float bodyRadius = 1.5f;
    float bodyHeight = 20.0f;
    float detailHeight = 0.5f;
    float topRadius = 0.5f;
    float topHeight = 3.0f;

    // Gambar base bertingkat
    drawBase(x, y, z);

    // Gambar badan tugu
    drawBody(x, y + baseHeight, z, bodyRadius, bodyHeight);

    // Gambar detail emas
    drawGoldDetail(x, y + baseHeight + bodyHeight, z, bodyRadius, detailHeight);

    // Gambar puncak
    drawTop(x, y + baseHeight + bodyHeight + detailHeight, z, topRadius, topHeight);
}

// Fungsi untuk menggambar piramida
void drawPyramid(float x, float y, float z, float baseSize, float height) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.6f, 0.6f, 0.6f); // Warna abu-abu

    glBegin(GL_TRIANGLES);

    // Sisi depan
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-baseSize / 2, 0.0f, baseSize / 2);
    glVertex3f(baseSize / 2, 0.0f, baseSize / 2);

    // Sisi kanan
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(baseSize / 2, 0.0f, baseSize / 2);
    glVertex3f(baseSize / 2, 0.0f, -baseSize / 2);

    // Sisi belakang
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(baseSize / 2, 0.0f, -baseSize / 2);
    glVertex3f(-baseSize / 2, 0.0f, -baseSize / 2);

    // Sisi kiri
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-baseSize / 2, 0.0f, -baseSize / 2);
    glVertex3f(-baseSize / 2, 0.0f, baseSize / 2);

    glEnd();

    // Dasar piramida
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.3f, 0.1f); // Warna lebih gelap untuk dasar
    glVertex3f(-baseSize / 2, 0.0f, baseSize / 2);
    glVertex3f(baseSize / 2, 0.0f, baseSize / 2);
    glVertex3f(baseSize / 2, 0.0f, -baseSize / 2);
    glVertex3f(-baseSize / 2, 0.0f, -baseSize / 2);
    glEnd();

    glPopMatrix();
}
// Fungsi untuk menggambar sebuah pilar
void drawPillar(float x, float y, float z, float radius, float height, bool isBroken) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.8f, 0.8f, 0.8f); // Warna abu-abu pilar
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, 30, 30);

    glPopMatrix();
}

// Fungsi untuk menggambar tangga
void drawStairs(float x, float y, float z, float width, float depth, float stepHeight, int stepCount) {
    for (int i = 0; i < stepCount; i++) {
        glPushMatrix();
        glTranslatef(x, y + i * stepHeight, z + i * depth); // Menyesuaikan posisi tiap anak tangga
        glScalef(width, stepHeight, depth); // Skala anak tangga
        glColor3f(0.5f, 0.5f, 0.5f); // Warna abu-abu untuk tangga
        glutSolidCube(1.0f);
        glPopMatrix();
    }
}

// Fungsi untuk menggambar patung manusia sederhana
void drawHuman(float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Kepala
    glColor3f(0.9f, 0.8f, 0.7f); // Warna kulit
    glPushMatrix();
    glTranslatef(0.0f, scale * 1.8f, 0.0f); // Kepala di atas badan
    glutSolidSphere(scale * 0.4f, 20, 20);
    glPopMatrix();

    // Leher
    glColor3f(0.9f, 0.8f, 0.7f); // Sama seperti kepala
    glPushMatrix();
    glTranslatef(0.0f, scale * 1.4f, 0.0f);
    glScalef(0.3f, 0.5f, 0.3f);
    glutSolidCube(scale);
    glPopMatrix();

    // Badan
    glColor3f(0.2f, 0.3f, 0.8f); // Baju biru
    glPushMatrix();
    glTranslatef(0.0f, scale * 0.9f, 0.0f);
    glScalef(1.0f, 1.5f, 0.5f);
    glutSolidCube(scale);
    glPopMatrix();

    // Tangan Kiri
    glColor3f(0.9f, 0.8f, 0.7f); // Warna kulit
    glPushMatrix();
    glTranslatef(-scale * 0.7f, scale * 1.1f, 0.0f);
    glRotatef(-15.0f, 0.0f, 0.0f, 1.0f); // Rotasi sedikit untuk posisi natural
    glScalef(0.3f, 1.0f, 0.3f);
    glutSolidCube(scale);
    glPopMatrix();

    // Tangan Kanan
    glPushMatrix();
    glTranslatef(scale * 0.7f, scale * 1.1f, 0.0f);
    glRotatef(15.0f, 0.0f, 0.0f, 1.0f); // Rotasi sedikit untuk posisi natural
    glScalef(0.3f, 1.0f, 0.3f);
    glutSolidCube(scale);
    glPopMatrix();

    // Kaki Kiri
    glColor3f(0.1f, 0.1f, 0.1f); // Celana hitam
    glPushMatrix();
    glTranslatef(-scale * 0.3f, -scale * 0.1f, 0.0f); // Sesuaikan posisi agar kaki menyatu dengan badan
    glScalef(0.3f, 1.5f, 0.3f); // Perbesar skala vertikal kaki
    glutSolidCube(scale);
    glPopMatrix();

    // Kaki Kanan
    glPushMatrix();
    glTranslatef(scale * 0.3f, -scale * 0.1f, 0.0f); // Sesuaikan posisi agar kaki menyatu dengan badan
    glScalef(0.3f, 1.5f, 0.3f); // Perbesar skala vertikal kaki
    glutSolidCube(scale);
    glPopMatrix();

    // Sepatu Kiri
    glColor3f(0.2f, 0.2f, 0.2f); // Sepatu abu-abu gelap
    glPushMatrix();
    glTranslatef(-scale * 0.3f, -scale * 0.85f, 0.0f); // Sesuaikan posisi sepatu dengan kaki
    glScalef(0.35f, 0.2f, 0.5f);
    glutSolidCube(scale);
    glPopMatrix();

    // Sepatu Kanan
    glPushMatrix();
    glTranslatef(scale * 0.3f, -scale * 0.85f, 0.0f); // Sesuaikan posisi sepatu dengan kaki
    glScalef(0.35f, 0.2f, 0.5f);
    glutSolidCube(scale);
    glPopMatrix();

    glPopMatrix();
}

void drawArchway(float x, float y, float z, float width, float height, float depth) {
    glPushMatrix();
    glTranslatef(x, y, z); // Posisi archway secara keseluruhan

    // Warna krem untuk seluruh archway
    glColor3f(0.8f, 0.7f, 0.6f);

    // Balok Vertikal Kiri 
    glPushMatrix();
    glTranslatef(-width / 2.0f + width * 0.05f, height / 2.0f, 0.0f); // Posisi kiri
    glScalef(width * 0.1f, height * 0.8f, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Balok Vertikal Kanan 
    glPushMatrix();
    glTranslatef(width / 2.0f - width * 0.05f, height / 2.0f, 0.0f); // Posisi kanan
    glScalef(width * 0.1f, height * 0.8f, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Balok Horizontal Atas 
    glPushMatrix();
    glTranslatef(0.0f, height - height * 0.1f, 0.0f); // Posisi atas
    glScalef(width * 0.9f, height * 0.1f, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Sudut Melengkung Kiri
    glPushMatrix();
    glTranslatef(-width / 2.0f + width * 0.05f, height - height * 0.05f, 0.0f); // Posisi sudut kiri atas
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotasi agar melengkung menghadap ke depan
    glColor3f(0.8f, 0.7f, 0.6f); // Warna sudut melengkung
    GLUquadric* leftCorner = gluNewQuadric();
    gluPartialDisk(leftCorner, 0.0f, width * 0.1f, 32, 1, 0.0f, 180.0f); // Setengah lingkaran
    gluDeleteQuadric(leftCorner);
    glPopMatrix();

    // Sudut Melengkung Kanan
    glPushMatrix();
    glTranslatef(width / 2.0f - width * 0.05f, height - height * 0.05f, 0.0f); // Posisi sudut kanan atas
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotasi agar melengkung menghadap ke depan
    glColor3f(0.8f, 0.7f, 0.6f); // Warna sudut melengkung
    GLUquadric* rightCorner = gluNewQuadric();
    gluPartialDisk(rightCorner, 0.0f, width * 0.1f, 32, 1, 0.0f, 180.0f); // Setengah lingkaran
    gluDeleteQuadric(rightCorner);
    glPopMatrix();

    glPopMatrix(); // Kembalikan transformasi global
}

// Fungsi untuk menggambar monumen pahlawan
void drawHeroMonument(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z); // Pindahkan monumen sesuai parameter posisi

    // Rotasi seluruh monumen untuk membelakangi tugu utama
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotasi 180 derajat di sekitar sumbu Y

    // Tangga
    drawStairs(0.0f, 0.0f, 0.0f, 6.0f, 0.5f, 0.3f, 10);

    // Pilar-pilar di sekitar monumen
    drawPillar(-3.0f, 1.5f, -2.0f, 0.3f, 5.0f, true);  // Pilar kiri belakang
    drawPillar(3.0f, 1.5f, -2.0f, 0.3f, 5.0f, true);   // Pilar kanan belakang
    drawPillar(-3.0f, 1.5f, 2.0f, 0.3f, 5.0f, false);  // Pilar kiri depan
    drawPillar(3.0f, 1.5f, 2.0f, 0.3f, 5.0f, false);   // Pilar kanan depan

    // Dasar monumen
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 7.0f);
    glScalef(5.0f, 6.0f, 5.0f);
    glColor3f(0.3f, 0.3f, 0.3f); // Warna abu-abu gelap
    glutSolidCube(1.0f);
    glPopMatrix();

    // Patung 
    drawHuman(-1.0f, 7.0f, 7.0f, 1.0f); // Patung kiri (pindahkan ke belakang tangga)
    drawHuman(1.0f, 7.0f, 7.0f, 1.0f);  // Patung kanan (pindahkan ke belakang tangga)

    // Gapura 1 - di belakang monumen
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 15.0f); // Pindahkan gapura ke belakang
    drawArchway(0.0f, 0.0f, 0.0f, 9.0f, 10.0f, 1.0f); // Gambar gapura
    glPopMatrix();

    // Gapura 2 - di samping kiri dengan rotasi
    glPushMatrix();
    glTranslatef(-10.0f, .0f, 10.0f); // Pindahkan gapura ke kiri
    glRotatef(135.0f, 0.0f, 1.0f, 0.0f); // Rotasi 90 derajat di sumbu Y
    drawArchway(0.0f, 0.0f, 0.0f, 6.0f, 10.0f, 1.0f); // Gambar gapura
    glPopMatrix();

    // Gapura 3 - di samping kanan dengan rotasi
    glPushMatrix();
    glTranslatef(10.0f, 0.0f, 10.0f); // Pindahkan gapura ke kanan
    glRotatef(-135.0f, 0.0f, 1.0f, 0.0f); // Rotasi -90 derajat di sumbu Y
    drawArchway(0.0f, 0.0f, 0.0f, 6.0f, 10.0f, 1.0f); // Gambar gapura
    glPopMatrix();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(15.0f, 20.0f, 40.0f,  // Posisi kamera lebih tinggi
        0.0f, 10.0f, 0.0f,    // Fokus pada tugu (y juga lebih tinggi)
        0.0f, 1.0f, 0.0f);    // Arah atas

    // Terapkan translasi global di sumbu Y
    glTranslatef(translateX, translateY, 0.0f);

    // Terapkan skala global
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Rotasi gedung
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);

    // Gambar tanah
    drawGround(0.0f, 0.1f, 15.0f, 110.0f, 110.0f, 0.5f);

    // Gambar dekorasi di sekitar base tugu
    drawBaseDecoration(0.0f, 0.5f, 0.0f, 15.0f); // Dekorasi di atas base tugu
    drawDetailedGround(0.0f, 0.1f, 0.0f, 100.0f, 100.0f);

    // Gambar tugu di tengah
    drawMonument(0.0f, 0.0f, 0.0f);

    drawHeroMonument(0.0f, 0.0f, 65.5f);
    // 3 Pohon di sisi kiri
    drawTree(0.0f, 0.0f, 0.0f, -27.5f); // Pohon 1 di kiri
    drawTree(0.0f, 0.0f, 6.0f, -27.5f); // Pohon 2 di kiri
    drawTree(0.0f, 0.0f, -6.0f, -27.5f);  // Pohon 3 di kiri

    // 3 Pohon di sisi kanan
    drawTree(0.0f, 0.0f, 0.0f, 27.5f);   // Pohon 1 di kanan
    drawTree(0.0f, 0.0f, 6.0f, 27.5f);  // Pohon 2 di kanan
    drawTree(0.0f, 0.0f, -6.0f, 27.5f);  // Pohon 3 di kanan
    // Tambahkan lapangan
    drawLapangan(0.0f, 0.6f, 35.0f, 100.0f, 30.0f);

    // Gambar pilar-pilar melingkar
    float radius = 14.0f; // Radius lingkaran untuk pilar
    float pillarHeight = 5.0f; // Tinggi pilar
    float pillarRadius = 0.5f; // Radius pilar
    int numPillars = 10; // Jumlah pilar
    for (int i = 0; i < numPillars; i++) {
        float angle = i * (360.0f / numPillars); // Sudut antar pilar
        float x = radius * cos(angle * M_PI / 180.0f);
        float z = radius * sin(angle * M_PI / 180.0f);
        drawPillar(x, 0.5f, z, pillarRadius, pillarHeight);
    }

    // Tambahkan 3 piramida di belakang tugu
    drawPyramid(-10.0f, 0.0f, -25.0f, 6.0f, 8.0f); // Piramida kiri
    drawPyramid(0.0f, 0.0f, -25.0f, 10.0f, 12.0f); // tengah
    drawPyramid(10.0f, 0.0f, -25.0f, 6.0f, 8.0f);  // Piramida kanan
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'x':
        angleX += 5.0f; // Rotasi di sekitar sumbu X
        if (angleX >= 360.0f) angleX -= 360.0f;
        break;
    case 'y':
        angleY += 5.0f; // Rotasi di sekitar sumbu Y
        if (angleY >= 360.0f) angleY -= 360.0f;
        break;
    case 'z':
        angleZ += 5.0f; // Rotasi di sekitar sumbu Z
        if (angleZ >= 360.0f) angleZ -= 360.0f;
        break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        translateY -= 0.5f;  // Geser objek ke atas
        break;
    case GLUT_KEY_DOWN:
        translateY += 0.5f;  // Geser objek ke bawah
        break;
    case GLUT_KEY_RIGHT:
        translateX += 0.5f;  // Geser objek ke kanan
        break;
    case GLUT_KEY_LEFT:
        translateX -= 0.5f;  // Geser objek ke kiri
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) { // Jika tombol mouse ditekan
        if (button == 3) { // Scroll up (zoom in)
            scaleFactor += 0.1f;
        }
        else if (button == 4) { // Scroll down (zoom out)
            if (scaleFactor > 0.1f)
                scaleFactor -= 0.1f;
        }
    }
    glutPostRedisplay(); // Meminta ulang rendering
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("TUGU PAHLAWAN 3D");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse); // Fungsi untuk menangani input mouse
    glutMainLoop();
    return 0;
}