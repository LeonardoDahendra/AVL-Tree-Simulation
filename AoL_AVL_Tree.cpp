#include<stdio.h>
#include<stdlib.h>

/*
Anggota:
	2602094080 - Flavia Louis
	2602097076 - Leonardo Dahendra
	2602097492 - Evelyn Angelica
	2602101514 - Valentcia Angelica
	2602101653 - Andrew Alfonso Lie
	2602172220 - Tiara Intan Kusuma
*/

//untuk AVL tree jika dimasukkan angka yang sama maka akan dimasukkan ke sebelah kanan

struct Node{
	//node menyimpan height agar height tidak perlu dihitung ulang setiap kali balancing
	int val, height;
	struct Node *left, *right;
}*root;

//char c hanya digunakan sebagai penyimpanan sementara dari hasil getchar()
char c;

//constructor untuk mempermudah pembuatan node baru
struct Node *createNode(int val){
	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->val = val;
	newNode->height = 0;
	newNode->left = newNode->right = NULL;
	
	return newNode;
}

//fungsi ini hanya digunakan agar setelah sebuah kode di run, tidak langsung keluar namun menunggu input dari user
void enterToContinue(){
	printf("Press enter to continue...\n");
	getchar();
}

//fungsi untuk mendapatkan nilai yang lebih besar dari 2 nilai yang dimasukkan
int max(int a, int b){
	return a > b ? a : b;
}

//fungsi untuk mendapatkan height dari node saat ini
int getHeight(struct Node *curRoot){
	if (curRoot == NULL) return -1;
	return curRoot->height;
}

//fungsi yang digunakan untuk mengupdate height dari node saat ini
//penting untuk diingat bahwa update height wajib dilakukan dari node paling bawah/node dengan depth paling tinggi
//karena update tidak mengecek apakah height dari node kiri kanan sudah terupdate namun langsung menggunakan heightnya
//kode ini tidak menyebabkan masalah karena pada bagian insert, update node dilakukan secara recursive dari node paling bawah
//jadi height pasti benar
int updateHeight(struct Node *curRoot){
	return max(getHeight(curRoot->left), getHeight(curRoot->right)) + 1;
}

//fungsi untuk mendapatkan balance factor dari sebuah node
int getBF(struct Node *curRoot){
	return getHeight(curRoot->left) - getHeight(curRoot->right);
}

//fungsi untuk print tree secara preorder
void preorder(struct Node *curRoot){
	if (curRoot != NULL){
		printf(" %d", curRoot->val);
		preorder(curRoot->left);
		preorder(curRoot->right);
	}
}

//print secara inorder
void inorder(struct Node *curRoot){
	if (curRoot != NULL){
		inorder(curRoot->left);
		printf(" %d", curRoot->val);
		inorder(curRoot->right);
	}
}

//print secara postorder
void postorder(struct Node *curRoot){
	if (curRoot != NULL){
		postorder(curRoot->left);
		postorder(curRoot->right);
		printf(" %d", curRoot->val);
	}
}

//fungsi untuk print tree sesuai dengan output pada soal
void traversal(){
	//jika tree belum ada, maka tidak ada yang bisa diprint
	if (!root) {
		printf("Tree is empty!\n");	
		return;
	}
	printf("Preorder:"); preorder(root); printf("\n");
	printf("Inorder:"); inorder(root); printf("\n");
	printf("Postorder:"); postorder(root); printf("\n");
}

//print secara inorder namun lebih mendetail, selain print data juga print height, bf, dan nilai dari left dan rightnya(jika ada)
void inorderDetail(struct Node *curRoot){
	if (curRoot != NULL){
		inorderDetail(curRoot->left);
		printf("| %-5d | %-6d | %-16d |", curRoot->val, curRoot->height, getBF(curRoot));
		curRoot->left ? printf(" %-4d |", curRoot->left->val) : printf(" -    |");
		curRoot->right ? printf(" %-5d |\n", curRoot->right->val) : printf(" -     |\n");
		inorderDetail(curRoot->right);
	}
}

//fungsi untuk memanggil print inorder secara detail dan sekaligus print header dan footer dari tabel
void printDetailedTree(){
	//jika root tidak ada, tree kosong(tidak ada yang bisa diprint)
	if (!root){
		printf("Tree is empty!\n");
		return;
	}
	printf(
	"====================================================\n"
	"| Value | Height | Balancing Factor | Left | Right |\n"
	"====================================================\n");
	inorderDetail(root);
	printf("====================================================\n");
}

//fungsi untuk mengecek apakah sebuah data(value) terdapat dalam tree, return 1 jika ada dan 0 jika tidak ada
int dataInTree(int data, struct Node *curRoot){
	//jika node sudah null maka value tidak ada
	if (!curRoot) return 0;
	
	if (data < curRoot->val) return dataInTree(data, curRoot->left);
	else if (data > curRoot->val) return dataInTree(data, curRoot->right);
	//jika value sama dengan data maka data terdapat dalam tree
	else return 1;
}

//fungsi untuk melakukan rotasi ke kanan
struct Node *rightRotation(struct Node *curRoot){
	//menyimpan node kiri, pada comment node kiri akan dibilang sebagai node besar, node yang dirotate akan dibilang sebagai node utama
	struct Node *left = curRoot->left;
	//menyimpan node kanan dari node kiri, akan ditulis sebagai node kecil
	struct Node *leftRight = left->right;
	
	//mengubah kanan dari node besar menjadi node utama
	left->right = curRoot;
	//mengubah kiri dari node utama menjadi node kecil
	curRoot->left = leftRight;
	
	//melakukan update height kepada node yang diputar, update height node utama wajib dilakukan sebelum node besar
	curRoot->height = updateHeight(curRoot);
	left->height = updateHeight(left);
	
	//node utama yang baru merupakan node besar, return node besar
	return left;
}

//melakukan rotasi ke kiri terhadap node
struct Node *leftRotation(struct Node *curRoot){
	//hampir sama dengan right rotation, namun node yang diambil terbalik(jika right rotation menggunakan kiri, left menggunakan kanan)
	struct Node *right = curRoot->right;
	struct Node *rightLeft = right->left;
	
	right->left = curRoot;
	curRoot->right = rightLeft;
	
	curRoot->height = updateHeight(curRoot);
	right->height = updateHeight(right);
	
	return right;
}

//fungsi untuk mengecek apakah node masih balance dan melakukan perubahan jika tidak balance
struct Node *balancing(struct Node *curRoot){
	if (!curRoot) return NULL;
	//height diupdate dulu sebelum dicek
	curRoot->height = updateHeight(curRoot);
	
	//menyimpan balancing factor dari node untuk mempermudah, jadi tidak perlu memanggil function tiap kali cek
	//dan juga lebih efisien karena tidak perlu melakukan komputasi berulang-ulang
	int bf = getBF(curRoot);
	
	//anak node ke left
	if (bf > 1){
		//jika anak dari anak node ke right(LR), maka kode dlm kondisi akan di run
		if (getBF(curRoot->left) < 0) curRoot->left = leftRotation(curRoot->left);
		//jika anak dari anak node ke left(LL), maka hanya kode dibwh yg dirun, jika LR kode dibwh tetap di run karena double rotation
		return rightRotation(curRoot);
	}
	//anak node ke right
	else if (bf < -1){
		//jika anak dari anak node ke left(RL), maka kode dlm kondisi akan di run
		if (getBF(curRoot->right) > 0) curRoot->right = rightRotation(curRoot->right);
		//jika anak dari anak node ke right(RR), maka hanya kode dibwh yg di run, jika RL kode dibwh tetap di run karena double rotation
		return leftRotation(curRoot);
	}
	
	//node yang direturn sama jika tidak terjadi perubahan(node masih balanced)
	return curRoot;
}

//fungsi untuk insert node baru, hampir sama persis dengan insert pada bst biasa
struct Node *insert(struct Node *curRoot, struct Node *newNode){
	//node yang pertama kali baru masuk pasti balanced dan heightnya sudah diset 0 secara default, maka tidak perlu dibalancing
	if (curRoot == NULL) return newNode;
	if (curRoot->val > newNode->val) curRoot->left = insert(curRoot->left, newNode);
	//jika terdapat nilai yang sama, dianggap ke kanan, menyesuaikan dengan visualisasi yang terdapat di internet
	else curRoot->right = insert(curRoot->right, newNode);
	//sebelum direturn nodenya, dicek dulu apakah node yang ingin direturn sudah balance
	return balancing(curRoot);
}

//fungsi untuk melakukan delete
struct Node *deleteNode(struct Node *curRoot, int deletVal){
	if (!curRoot) return NULL;
	
	if (deletVal > curRoot->val) curRoot->right = deleteNode(curRoot->right, deletVal);
	else if (deletVal < curRoot->val) curRoot->left = deleteNode(curRoot->left, deletVal);
	//jika node yang ingin dihapus ditemukan
	else{
		//node pengganti node yang ingin dihapus secara default diasumsi sebagai kiri dari node
		struct Node *newCurr = curRoot->left;
		//jika node mempunyai 2 anak
		if (curRoot->left && curRoot->right){
			//mencari pengganti menggunakan predecessor, yaitu mengambil node terbesar dari left subtree
			while(newCurr->right) newCurr = newCurr->right;
			//setelah ditemukan penggantinya, ubah nilai dari node sekarang menjadi nilai dari penggantinya
			curRoot->val = newCurr->val;
			//lalu penggantinya akan dihapus, jadi sebenarnya hanya bertukar value
			curRoot->left = deleteNode(curRoot->left, newCurr->val);
		}
		//jika node mempunya < 2 anak(0 atau 1)
		else{
			//jika node mempunyai anak kanan(berarti tidak mempunyai anak kiri), maka penggantinya diubah menjadi anak kanannya
			//jika kode ini tidak dirun, maka node mempunyai anak kiri atau tidak mempunyai anak
			if (curRoot->right) newCurr = curRoot->right;
			//free node sekarang agar tidak mengambil memory
			free(curRoot);
			//mengubah node sekarang menjadi node baru(bisa node kiri, bisa node kanan, bisa NULL)
			curRoot = newCurr;
		}
	}
	
	//sebelum direturn nodenya, dicek terlebih dahulu apakah sudah balanced
	return balancing(curRoot);
}

//menu untuk mendapatkan value yang ingin didelete
void deleteMenu(){
	//jika root tidak ada maka tree kosong(tidak ada data yang dapat didelete)
	if (!root){
		printf("Tree is empty!\n");
		return;
	}
	
	//variabel untuk menyimpan angka yang ingin didelete
	int deleteVal;

	printf("Delete: ");
	//tidak terdapat validasi untuk angka yang akan didelete karena tidak diminta
	scanf("%d", &deleteVal);
	//clear input buffer
	while((c = getchar()) != EOF && c != '\n'){}
	
	//jika data yang ingin dihapus user tidak terdapat dalam tree
	if (!dataInTree(deleteVal, root)){
		printf("Data not found\n");
		return;
	}
	//jika data terdapat dalam tree, maka delete nodenya
	printf("Data Found\n");
	root = deleteNode(root, deleteVal);
	printf("Value %d was deleted\n", deleteVal);
}

//fungsi menu untuk meminta input yang ingin dimasukkan dari user
void insertNode(){
	int val;
	
	//tidak terdapat validasi untuk angka yang dimasukkan karena soal tidak meminta validasi
	printf("Insert: ");
	scanf("%d", &val);
	//kode ini mirip dengan getLine pada java, untuk mendapatkan semua input buffer sisa dari scanf
	while((c = getchar()) != EOF && c != '\n'){}
	
	//memanggil insert setelah mendapatkan value dari user
	root = insert(root, createNode(val));
	
	printf("Value inserted into tree\n");
}

int main(){
	//untuk hasil traversal inorder dan postorder berbeda dari hasil pada soal karena hasil pada soal salah
	int dataInsertSoal[] = {6, 27, 19, 11, 36, 14, 81, 63, 75};
	int dataDeleteSoal[] = {14, 75, 36, 19, 11};
	
	//jika tidak ingin tree awal terisi, bisa comment atau hapus kedua for loop ini
	for (int i = 0; i < sizeof(dataInsertSoal) / sizeof(dataInsertSoal[0]); i++){
		root = insert(root, createNode(dataInsertSoal[i]));
	}
	for (int i = 0; i < sizeof(dataDeleteSoal) / sizeof(dataDeleteSoal[0]); i++){
		root = deleteNode(root, dataDeleteSoal[i]);
	}
	
	//variabel untuk menyimpan input pilihan dari user
	int choice = -1;
	do{
		//clear layar agar lebih enak dilihat
		system("cls");
		printf("1. Insertion\n"
		"2. Deletion\n"
		"3. Traversal\n"
		"4. Show Detailed Tree\n"
		"5. Exit\nChoose:  ");
		
		scanf("%d", &choice);
		//clear input buffer
		while((c = getchar()) != EOF && c != '\n'){}
		
		//switch untuk setiap pilihan yang dapat dipilih user
		switch(choice){
		case 1:
			insertNode();
			break;
		case 2:
			deleteMenu();
			break;
		case 3:
			traversal();
			break;
		case 4:
			//tambahan fitur agar dapat melihat setiap node secara lebih detail
			printDetailedTree();						
			break;
		}
		//klo user pencet 5, lsg kluar dri loop sja
		if (choice != 5) enterToContinue();		
		//selama user belum memilih untuk exit, maka program akan terus di run
	}while(choice != 5);
	//setelah user memilih exit, maka akan print
	printf("Thank you\n");
	//agar jika di run diluar IDE, setelah selesai tidak langsung diclose
	enterToContinue();
	return 0;
}
