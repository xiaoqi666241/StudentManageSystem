#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//最大学生数,最多科目数
#define MAX_STUDENTS 1000
#define MAX_SUBJECTS 10
//新建文档
#define FILENAME "students.text"

// 学生结构体
typedef struct {
    char id[20];
    char name[50];
    float scores[MAX_SUBJECTS];
    float total;
    float average;
    int subject_count;
} Student;

// 全局变量
Student students[MAX_STUDENTS];
int student_count = 0;
int subject_total = 5;  // 默认5门科目

// 函数声明
void display_menu();

//新增学生信息 王梦涵 
void add_student();
//修改学生信息王梦涵 
void modify_student();
 //删除学生信息王梦涵 
void delete_student();
//显示所有学生信息 田维先
void display_all();
// 按总分排序（降序）  田维先
void sort_by_total();
//按学号排序  田维先
void sort_by_id();
//// 按学号查询学生信息  朱振奥
void search_by_id();
//保存到文档 朱振奥
void save_to_file();
//加载  朱振奥
void load_from_file();
//分析成绩  喻梦琪
void analyze_student();
//以下几个为全局函数,封怡晨
int find_student_index(const char* id);
void calculate_student_stats(Student* stu);
int confirm_action();
void clear_input_buffer();

// 主函数
int main() {
    //
    load_from_file();

    int choice;
    while(1) {
        display_menu();
        printf("请输入您的选择 (0-9): ");
        //只要读入了数就true
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("输入无效，请重新输入！\n");
            continue;
        }
        clear_input_buffer();

        switch (choice) {
        case 1: add_student(); break;
        case 2: modify_student(); break;
        case 3: delete_student(); break;
        case 4: display_all(); break;
        case 5: sort_by_total(); break;
        case 6: sort_by_id(); break;
        case 7: search_by_id(); break;
        case 8: analyze_student(); break;
        case 9: printf("正在保存数据...\n"); save_to_file(); break;
        case 0:
            printf("确定要退出吗？(y/n): ");
            if (confirm_action()) {
                printf("正在保存数据并退出...\n");
                save_to_file();
                printf("感谢使用学生成绩管理系统！\n");
                return 0;
            }
            break;
        default: printf("无效的选择，请重新输入！\n");
        }
        printf("\n按Enter键继续...");
        getchar();
    } 

    return 0;
}

// 显示菜单
void display_menu() {
    system("clear || cls");  // 清屏,将以前的代码痕迹消除
    printf("\n========== 学生成绩管理系统 ==========\n");
    printf("1. 新增学生信息\n");
    printf("2. 修改学生信息\n");
    printf("3. 删除学生信息\n");
    printf("4. 显示所有学生成绩\n");
    printf("5. 按总分排序\n");
    printf("6. 按学号排序\n");
    printf("7. 按学号查询学生信息\n");
    printf("8. 分析学生成绩\n");
    printf("9. 保存数据到文件\n");
    printf("0. 退出系统\n");
    printf("======================================\n");
}

// 清除输入缓冲区
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF){
        //空则表示继续,只有不满足条件才结束循环
    }
}

// 确认操作
int confirm_action() {
    char choice;
    scanf(" %c", &choice);
    clear_input_buffer();
    return (tolower(choice) == 'y');
}

// 计算学生统计总分和平均分
void calculate_student_stats(Student* stu) {
    stu->total = 0;
    for (int i = 0; i < stu->subject_count; i++) {
        stu->total += stu->scores[i];
    }
    stu->average = stu->total / stu->subject_count;
}

// 查找学生索引
int find_student_index(const char* id) {
    for (int i = 0; i < student_count; i++) {
        //strcmp不仅仅可以比较字典序,还可以比较数字字符串
        if (strcmp(students[i].id, id) == 0) {
            return i;
        }
    }
    return -1;  // 未找到
}

// 新增学生 王梦涵
void add_student() {
    if (student_count >= MAX_STUDENTS) {
        printf("学生数量已达上限！\n");
        return;
    }

    Student new_student;

    printf("\n--- 新增学生信息 ---\n");

    // 输入学号
    while (1) {
        printf("请输入学号: ");
        //stdin是常见用法,从键盘输入
        fgets(new_student.id, sizeof(new_student.id), stdin);
        //strcspn(tr1,str2)从str1开头开始查找,有多少个字符不属于str2,返回此数
        //主要功能是将\n换成'\0'
        new_student.id[strcspn(new_student.id, "\n")] = '\0';  // 去除换行符

        if (find_student_index(new_student.id) != -1) {
            printf("该学号已存在！请重新输入。\n");
        }
        else {
            break;
        }
    }

    // 输入姓名
    printf("请输入姓名: ");
    fgets(new_student.name, sizeof(new_student.name), stdin);
    new_student.name[strcspn(new_student.name, "\n")] = 0;

    // 输入科目数
    printf("请输入考试科目数量 (最多%d门): ", MAX_SUBJECTS);
    scanf("%d", &new_student.subject_count);
    clear_input_buffer();

    if (new_student.subject_count > MAX_SUBJECTS || new_student.subject_count <= 0) {
        printf("科目数量无效！\n");
        return;
    }

    // 输入各科成绩
    printf("\n请输入各科成绩:\n");
    for (int i = 0; i < new_student.subject_count; i++) {
        printf("科目%d成绩: ", i + 1);
        scanf("%f", &new_student.scores[i]);
        clear_input_buffer();
    }

    // 计算总分和平均分
    calculate_student_stats(&new_student);

    // 添加到数组
    students[student_count++] = new_student;

    printf("\n学生信息添加成功！\n");
    printf("学号: %s, 姓名: %s, 总分: %.2f, 平均分: %.2f\n",
        new_student.id, new_student.name, new_student.total, new_student.average);
}

// 修改学生信息  王梦涵
void modify_student() {
    char id[20];
    printf("\n请输入要修改的学生学号: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int index = find_student_index(id);
    if (index == -1) {
        printf("未找到学号为 %s 的学生！\n", id);
        return;
    }

    printf("\n找到学生: %s - %s\n", students[index].id, students[index].name);
    printf("当前成绩: ");
    for (int i = 0; i < students[index].subject_count; i++) {
        printf("科目%d: %.1f ", i + 1, students[index].scores[i]);
    }
    printf("\n");
    printf("确定要修改该学生信息吗？(y/n): ");
    if (!confirm_action()) {
        printf("操作已取消。\n");
        return;
    }

    printf("\n--- 修改学生信息 ---\n");

    // 修改姓名
    printf("请输入新姓名 (原: %s): ", students[index].name);
    char new_name[50];
    fgets(new_name, sizeof(new_name), stdin);
    new_name[strcspn(new_name, "\n")] = 0;
    if (strlen(new_name) > 0) {
        strcpy(students[index].name, new_name);
    }

    // 修改各科成绩
    printf("\n修改各科成绩 (输入负数跳过修改):\n");
    for (int i = 0; i < students[index].subject_count; i++) {
        float new_score;
        printf("科目%d新成绩 (原: %.1f): ", i + 1, students[index].scores[i]);
        scanf("%f", &new_score);
        clear_input_buffer();

        if (new_score >= 0 && new_score <= 100) {
            students[index].scores[i] = new_score;
        }
    }

    // 重新计算
    calculate_student_stats(&students[index]);

    printf("\n学生信息修改成功！\n");
}

// 删除学生信息  王梦涵
void delete_student() {
    char id[20];
    printf("\n请输入要删除的学生学号: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int index = find_student_index(id);
    if (index == -1) {
        printf("未找到学号为 %s 的学生！\n", id);
        return;
    }

    printf("\n找到学生: %s - %s\n", students[index].id, students[index].name);
        printf("确定要删除该学生吗？此操作不可恢复！(y/n): ");
    if (!confirm_action()) {
        printf("操作已取消。\n");
        return;
    }

    // 二次确认
    printf("请再次确认删除操作 (y/n): ");
    if (!confirm_action()) {
        printf("操作已取消。\n");
        return;
    }

    // 删除学生（前移数组元素）
    for (int i = index; i < student_count - 1; i++) {
        students[i] = students[i + 1];
    }
    student_count--;

    printf("学生信息删除成功！\n");
}


// 显示所有学生信息  田维先
void display_all() {
    if (student_count == 0) {
        printf("暂无学生信息！\n");
        return;
    }

    printf("\n=== 所有学生成绩信息 ===\n");
    printf("%-15s %-20s ", "学号", "姓名");

    for (int i = 0; i < subject_total; i++) {
        printf("科目%d ", i + 1);
    }

    printf("%-10s %-10s\n", "总分", "平均分");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < student_count; i++) {
        printf("%-15s %-20s ", students[i].id, students[i].name);

        for (int j = 0; j < students[i].subject_count; j++) {
            printf("%-6.1f ", students[i].scores[j]);
        }

        // 如果科目数少于总科目数，填充空格
        for (int j = students[i].subject_count; j < subject_total; j++) {
            printf("%-6s ", "-");
        }

        printf("%-10.1f %-10.2f\n", students[i].total, students[i].average);
    }
}





// 按总分排序（降序）  田维先
void sort_by_total() {
    if (student_count == 0) {
        printf("暂无学生信息！\n");
        return;
    }

    // 使用冒泡排序
    for (int i = 0; i < student_count - 1; i++) {
        for (int j = 0; j < student_count - i - 1; j++) {
            if (students[j].total < students[j + 1].total) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    printf("按总分排序完成！\n");
    display_all();
}

// 按学号排序（升序）田维先
void sort_by_id() {
    if (student_count == 0) {
        printf("暂无学生信息！\n");
        return;
    }

    // 使用冒泡排序
    for (int i = 0; i < student_count - 1; i++) {
        for (int j = 0; j < student_count - i - 1; j++) {
            if (strcmp(students[j].id, students[j + 1].id) > 0) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    printf("按学号排序完成！\n");
    display_all();
}

// 按学号查询   朱振奥
void search_by_id() {
    char id[20];
    printf("\n请输入要查询的学生学号: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int index = find_student_index(id);
    if (index == -1) {
        printf("未找到学号为 %s 的学生！\n", id);
        return;
    }

    Student* stu = &students[index];

    printf("\n=== 学生基本信息 ===\n");
    printf("学号: %s\n", stu->id);
    printf("姓名: %s\n", stu->name);
    printf("各科成绩:\n");

    for (int i = 0; i < stu->subject_count; i++) {
        printf("  科目%d: %.1f\n", i + 1, stu->scores[i]);
    }

    printf("总分: %.1f\n", stu->total);
    printf("平均分: %.2f\n", stu->average);

    // 显示大致排名（需要先按总分排序）
    int rank = 1;
    for (int i = 0; i < student_count; i++) {
        if (students[i].total > stu->total) {
            rank++;
        }
    }

    printf("大致排名: %d/%d\n", rank, student_count);
}


// 保存数据到文件  朱振奥
void save_to_file() {
    FILE* file = fopen(FILENAME, "w");  // "w" 文本模式
    if (file == NULL) {
        printf("无法打开文件进行保存！\n");
        return;
    }

    // 文本格式保存
    fprintf(file, "%d %d\n", student_count, subject_total);
    
    for (int i = 0; i < student_count; i++) {
        fprintf(file, "%s %s %d ", students[i].id, students[i].name, 
                students[i].subject_count);
        
        for (int j = 0; j < students[i].subject_count; j++) {
            fprintf(file, "%.2f ", students[i].scores[j]);
        }
        fprintf(file, "%.2f %.2f\n", students[i].total, students[i].average);
    }
    
    fclose(file);
    printf("数据已保存到文本文件: %s\n", FILENAME);
}

// 从文件加载数据  朱振奥
void load_from_file() {
    FILE* file = fopen(FILENAME, "r");  // "r" 只读文本模式
    if (file == NULL) {
        printf("未找到数据文件，将使用空文件。\n");
        return;
    }

    // 文本格式读取
    fscanf(file, "%d %d", &student_count, &subject_total);
    
    for (int i = 0; i < student_count; i++) {
        fscanf(file, "%s %s %d", 
               students[i].id, 
               students[i].name, 
               &students[i].subject_count);
        
        for (int j = 0; j < students[i].subject_count; j++) {
            fscanf(file, "%f", &students[i].scores[j]);
        }
        fscanf(file, "%f %f", &students[i].total, &students[i].average);
    }
    
    fclose(file);
    printf("从文本文件加载了 %d 个学生数据\n", student_count);
}


// 分析学生成绩  喻梦琪
void analyze_student() {
    char id[20];
    printf("\n请输入要分析的学生学号: ");
    gets(id);

    int index = find_student_index(id);
    if (index == -1) {
        printf("未找到学号为 %s 的学生！\n", id);
        return;
    }

    Student *stu = &students[index];

    printf("\n 学生成绩分析报告 \n");
    printf("学生: %s - %s\n\n", students[index].id, students[index].name);

    // 单科排名分析
    printf("1. 单科成绩排名分析:\n");
    for (int subject = 0; subject < students[index].subject_count; subject++) {
        int rank = 1;
        for (int i = 0; i < student_count; i++) {
            if (students[i].subject_count > subject &&
                students[i].scores[subject] > students[index].scores[subject]) {
                rank++;
            }
        }
        printf("   科目%d: %.1f分，排名 %d/%d\n",
            subject + 1, students[index].scores[subject], rank, student_count);
    }

    // 优势学科和劣势学科分析
    printf("\n2. 优势与劣势学科分析:\n");

    int best_subject = 0, worst_subject = 0;
    float best_score = students[index].scores[0];
    float worst_score = students[index].scores[0];
    //遍历查找学生的所有科目的成绩，并找出最高分和最低分科目
    for (int i = 1; i < stu->subject_count; i++) {
        if (students[index].scores[i] > best_score) {
            best_score = students[index].scores[i];
            best_subject = i;
        }
        if (stu->scores[i] < worst_score) {
            worst_score = stu->scores[i];
            worst_subject = i;
        }
    }

    printf("优势学科: 科目%d (%.1f分)\n", best_subject + 1, best_score);
    printf("劣势学科: 科目%d (%.1f分)\n", worst_subject + 1, worst_score);

    // 与平均分比较
    printf("\n3. 与平均分比较:\n");
    for (int i = 0; i < stu->subject_count; i++) {
        float diff = stu->scores[i] - stu->average;
        if (diff > 0) {
            printf("   科目%d: 高于平均分 %.1f分\n", i + 1, diff);
        }
        else if (diff < 0) {
            printf("   科目%d: 低于平均分 %.1f分\n", i + 1, -diff);
        }
        else {
            printf("   科目%d: 等于平均分\n", i + 1);
        }
    }
}
















