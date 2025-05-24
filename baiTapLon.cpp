#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Cấu trúc sinh viên
typedef struct Student {
    char id[10];
    char name[50];
    float gpa;
    struct Student* next; // Con trỏ cho danh sách liên kết
} Student;

// Cấu trúc danh sách liên kết
typedef struct {
    Student* head;
    int size;
} StudentList;

// Hàm khởi tạo danh sách
StudentList* initList() {
    StudentList* list = (StudentList*)malloc(sizeof(StudentList));
    list->head = NULL;
    list->size = 0;
    return list;
}

// Hàm thêm sinh viên
void addStudent(StudentList* list) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    printf("\nNhap ma so sinh vien: ");
    scanf("%s", newStudent->id);
    printf("Nhap ho ten: ");
    getchar(); // Xóa buffer
    fgets(newStudent->name, 50, stdin);
    newStudent->name[strcspn(newStudent->name, "\n")] = 0; // Xóa ký tự newline
    printf("Nhap diem trung binh: ");
    scanf("%f", &newStudent->gpa);
    newStudent->next = NULL;

    if (list->head == NULL) {
        list->head = newStudent;
    } else {
        newStudent->next = list->head;
        list->head = newStudent;
    }
    list->size++;
    printf("Them sinh vien thanh cong!\n");
}

// Hàm hiển thị danh sách sinh viên
void displayList(StudentList* list) {
    if (list->size == 0) {
        printf("\nDanh sach rong!\n");
        return;
    }
    printf("\nDanh sach sinh vien:\n");
    printf("%-10s %-30s %-10s\n", "Ma SV", "Ho ten", "Diem TB");
    printf("----------------------------------------\n");
    Student* current = list->head;
    while (current != NULL) {
        printf("%-10s %-30s %-10.2f\n", current->id, current->name, current->gpa);
        current = current->next;
    }
}

// Hàm tìm kiếm sinh viên theo mã
Student* findStudent(StudentList* list, char* id) {
    Student* current = list->head;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Hàm xóa sinh viên
void deleteStudent(StudentList* list) {
    char id[10];
    printf("\nNhap ma so sinh vien can xoa: ");
    scanf("%s", id);
    
    Student* current = list->head;
    Student* prev = NULL;
    
    while (current != NULL && strcmp(current->id, id) != 0) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("Khong tim thay sinh vien!\n");
        return;
    }
    
    if (prev == NULL) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }
    
    free(current);
    list->size--;
    printf("Xoa sinh vien thanh cong!\n");
}

// Hàm sửa thông tin sinh viên
void editStudent(StudentList* list) {
    char id[10];
    printf("\nNhap ma so sinh vien can sua: ");
    scanf("%s", id);
    
    Student* student = findStudent(list, id);
    if (student == NULL) {
        printf("Khong tim thay sinh vien!\n");
        return;
    }
    
    printf("Nhap ho ten moi: ");
    getchar();
    fgets(student->name, 50, stdin);
    student->name[strcspn(student->name, "\n")] = 0;
    printf("Nhap diem trung binh moi: ");
    scanf("%f", &student->gpa);
    printf("Cap nhat thong tin thanh cong!\n");
}

// Hàm sắp xếp danh sách theo điểm trung bình
void sortList(StudentList* list, int ascending) {
    if (list->size <= 1) return;
    
    Student *current, *index;
    char tempId[10], tempName[50];
    float tempGpa;
    
    for (current = list->head; current != NULL; current = current->next) {
        for (index = current->next; index != NULL; index = index->next) {
            if ((ascending && current->gpa > index->gpa) || 
                (!ascending && current->gpa < index->gpa)) {
                // Hoán đổi thông tin
                strcpy(tempId, current->id);
                strcpy(tempName, current->name);
                tempGpa = current->gpa;
                
                strcpy(current->id, index->id);
                strcpy(current->name, index->name);
                current->gpa = index->gpa;
                
                strcpy(index->id, tempId);
                strcpy(index->name, tempName);
                index->gpa = tempGpa;
            }
        }
    }
    printf("Sap xep danh sach thanh cong!\n");
}

// Hàm lưu danh sách vào file
void saveToFile(StudentList* list) {
    FILE* file = fopen("students.txt", "w");
    if (file == NULL) {
        printf("Khong the mo file!\n");
        return;
    }
    
    Student* current = list->head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%.2f\n", current->id, current->name, current->gpa);
        current = current->next;
    }
    
    fclose(file);
    printf("Luu danh sach vao file thanh cong!\n");
}

// Hàm đọc danh sách từ file
void loadFromFile(StudentList* list) {
    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("Khong the mo file hoac file khong ton tai!\n");
        return;
    }
    
    // Xóa danh sách hiện tại
    Student* current = list->head;
    while (current != NULL) {
        Student* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->size = 0;
    
    char id[10], name[50];
    float gpa;
    while (fscanf(file, "%[^,],%[^,],%f\n", id, name, &gpa) == 3) {
        Student* newStudent = (Student*)malloc(sizeof(Student));
        strcpy(newStudent->id, id);
        strcpy(newStudent->name, name);
        newStudent->gpa = gpa;
        newStudent->next = NULL;
        
        if (list->head == NULL) {
            list->head = newStudent;
        } else {
            newStudent->next = list->head;
            list->head = newStudent;
        }
        list->size++;
    }
    
    fclose(file);
    printf("Doc danh sach tu file thanh cong!\n");
}

// Hàm thống kê sinh viên có điểm trên 8.0
void statsAboveEight(StudentList* list) {
    int count = 0;
    Student* current = list->head;
    while (current != NULL) {
        if (current->gpa >= 8.0) {
            count++;
        }
        current = current->next;
    }
    printf("\nSo luong sinh vien co diem trung binh >= 8.0: %d\n", count);
}

// Hàm giải phóng danh sách
void freeList(StudentList* list) {
    Student* current = list->head;
    while (current != NULL) {
        Student* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

// Hàm hiển thị menu
void displayMenu() {
    printf("\n=== QUAN LY SINH VIEN ===\n");
    printf("1. Them sinh vien\n");
    printf("2. Hien thi danh sach\n");
    printf("3. Tim kiem sinh vien\n");
    printf("4. Sua thong tin sinh vien\n");
    printf("5. Xoa sinh vien\n");
    printf("6. Sap xep danh sach (tang dan theo diem)\n");
    printf("7. Sap xep danh sach (giam dan theo diem)\n");
    printf("8. Luu danh sach vao file\n");
    printf("9. Doc danh sach tu file\n");
    printf("10. Thong ke sinh vien diem >= 8.0\n");
    printf("0. Thoat\n");
    printf("Nhap lua chon: ");
}

int main() {
    StudentList* list = initList();
    int choice;
    
    do {
        displayMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addStudent(list);
                break;
            case 2:
                displayList(list);
                break;
            case 3: {
                char id[10];
                printf("\nNhap ma so sinh vien can tim: ");
                scanf("%s", id);
                Student* student = findStudent(list, id);
                if (student) {
                    printf("%-10s %-30s %-10s\n", "Ma SV", "Ho ten", "Diem TB");
                    printf("----------------------------------------\n");
                    printf("%-10s %-30s %-10.2f\n", student->id, student->name, student->gpa);
                } else {
                    printf("Khong tim thay sinh vien!\n");
                }
                break;
            }
            case 4:
                editStudent(list);
                break;
            case 5:
                deleteStudent(list);
                break;
            case 6:
                sortList(list, 1); // Sắp xếp tăng dần
                displayList(list);
                break;
            case 7:
                sortList(list, 0); // Sắp xếp giảm dần
                displayList(list);
                break;
            case 8:
                saveToFile(list);
                break;
            case 9:
                loadFromFile(list);
                break;
            case 10:
                statsAboveEight(list);
                break;
            case 0:
                printf("Tam biet!\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
        }
    } while (choice != 0);
    
    freeList(list);
    return 0;
}