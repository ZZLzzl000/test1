#include <iostream>
#include<fstream>
#include<cstring>
#include<vector>
using namespace std;

#define FILENAME "stdFILE.txt" //定义生成文件名称

//学生类(抽象类)
class Student {
public:
    virtual void showInfo() = 0; //显示个人信息
    // virtual ~Student() = 0;      //定义纯虚析构函数保证释放堆区内存时不会发生内存泄漏

    string StId;     //学号
    string m_name;  //姓名
    string m_Dept;  //职责
};
//普通学生类
class Nomalstuden : public Student {
public:
    Nomalstuden(string Id, string name, string dep);
    void showInfo(); //显示个人信息
    // ~Nomalstuden() {
    //     delete this;
    // }
};
//班长类
class ClassPresident : public Student {
public:
    ClassPresident(string Id, string name, string dep);
    void showInfo();
    // ~ClassPresident() {
    //     delete this;
    // }
};
//班干部类
class Classleader : public Student {
public:
    Classleader(string Id, string name, string dep);
    void showInfo();
    // ~Classleader() {
    //     delete this;
    // }
};

//管理员类
class StudentManager {
public:
    StudentManager();//构造函数
    void Show_Menu(); //打印菜单界面
    void Exit_System();//退出系统
    void Addinfo();    //增加学生信息
    void save();       //将学生信息保存到文件中
    void init_Std();   //初始化学生
    void show_Std();   //显示学生信息
    void del_Std();    //删除学生信息
    void mod_Std();    //修改学生信息
    void find_Std();   //查找学生信息
    void clean_File(); //清空文件
    int IsExist(string id);     //判断学号为id的学生信息是否存在，并返回该学生下标
    ~StudentManager();//析构函数

    vector<Student*>* m_StdArray; //存放增加的学生信息
    bool m_fileIsEmpty;     //标记文件是否为空
};

//学生类纯虚析构的外部实现
// Student :: ~Student() {
//     delete this;
// }

//管理员函数实现
StudentManager::StudentManager() {
    ifstream ifs;
    ifs.open(FILENAME, ios::in);
    //如果文件不存在
    if (!ifs.is_open()) {
        cout << "该文件不存在！" << endl;
        this->m_fileIsEmpty = true;
        this->m_StdArray = NULL;
        ifs.close();
        return;
    }
    //文件存在但是数据为空
    char ch;
    ifs >> ch;       //先读取一个字符
    if (ifs.eof()) {
        cout << "该文件为空！" << endl;
        this->m_fileIsEmpty = true;
        this->m_StdArray = NULL;
        ifs.close();
        return;
    }
    //文件存在，并记录初始数据
    this->m_StdArray = new vector<Student*>;
    this->init_Std();
}
void StudentManager::Show_Menu() {
    cout << "-------------------------------------------" << endl;
    cout << "------------  欢迎使用学生管理系统！ ----------" << endl;
    cout << "-------------  0.退出管理程序  -------------" << endl;
    cout << "-------------  1.增加学生信息  -------------" << endl;
    cout << "-------------  2.显示学生信息  -------------" << endl;
    cout << "-------------  3.删除学生信息  -------------" << endl;
    cout << "-------------  4.修改学生信息  -------------" << endl;
    cout << "-------------  5.查找学生信息  -------------" << endl;
    cout << "-------------  6.清空所有数据  -------------" << endl;
    cout << "-------------------------------------------" << endl;
    cout << endl;
}
void StudentManager::Exit_System() {
    cout << "感谢您的使用！" << endl;
    // system("pause");
    exit(-1);  //退出系统
}
void StudentManager::Addinfo() {
    if (!this->m_StdArray)
        this->m_StdArray = new vector<Student*>;
    cout << "学生信息开始录入" << endl;
    int i = 1;
    while (true) {
        char flag;
        string id;
        string name;
        string dep;
        cout << "请输入第" << i << "个学生学号：" << endl;
        cin >> id;
        cout << "请输入第" << i << "个学生姓名：" << endl;
        cin >> name;
        cout << "请输入第" << i << "个学生职位：(班长or班干部or普通学生)" << endl;
        cin >> dep;
        Student* std = NULL;
        if (dep == "班长") {
            std = new ClassPresident(id, name, dep);
        }
        else if (dep == "班干部") {
            std = new Classleader(id, name, dep);
        }
        else if (dep == "普通学生") {
            std = new Nomalstuden(id, name, dep);
        }
        else {
            cout << "该学生职位不存在！信息录入结束！" << endl;
            break;
        }
        this->m_StdArray->push_back(std);
        i++;
        this->m_fileIsEmpty = false;   //更新文件非空标记
        cout << "是否继续录入信息？(y继续录入，n结束录入)" << endl;
        cin >> flag;
        if (flag == 'y') continue;
        else break;
    }
    cout << "成功添加了" << i - 1 << "名学生信息!" << endl;
    this->save();
    system("pause");
    system("cls");
}
void StudentManager::save() {
    ofstream ofs;
    ofs.open(FILENAME, ios::out);
    for (int i = 0; i < this->m_StdArray->size(); ++i) {
        ofs << this->m_StdArray->at(i)->StId << " "
            << this->m_StdArray->at(i)->m_name << " "
            << this->m_StdArray->at(i)->m_Dept << endl;
    }
    ofs.close();
}
void StudentManager::init_Std() {
    ifstream ifs;
    ifs.open(FILENAME, ios::in);
    string id;
    string name;
    string dep;
    while (ifs >> id && ifs >> name && ifs >> dep) {
        Student* std = NULL;
        if (dep == "班长") {
            std = new ClassPresident(id, name, dep);
        }
        else if (dep == "班干部") {
            std = new Classleader(id, name, dep);
        }
        else if (dep == "普通学生") {
            std = new Nomalstuden(id, name, dep);
        }
        this->m_StdArray->push_back(std);
    }
    this->save();
}
void StudentManager::show_Std() {
    if (this->m_fileIsEmpty) {
        cout << "文件不存在或者文件为空！" << endl;
    }
    else {
        for (int i = 0; i < this->m_StdArray->size(); ++i) {
            this->m_StdArray->at(i)->showInfo();
        }
    }
    system("pause");
    system("cls");
}
void StudentManager::del_Std() {
    if (this->m_fileIsEmpty) {
        cout << "文件不存在或者文件为空！" << endl;
    }
    else {
        cout << "请输入需要删除的学生学号：" << endl;
        string id;
        cin >> id;
        int index = this->IsExist(id);
        if (index != -1) {
            this->m_StdArray->erase(this->m_StdArray->begin() + index);
            this->save();
            cout << "删除成功！" << endl;
        }
        else {
            cout << "删除失败，不存在该学号的学生！" << endl;
        }
    }
    system("pause");
}
int StudentManager::IsExist(string id) {
    int len = this->m_StdArray->size();
    int index = -1;
    for (int i = 0; i < len; ++i) {
        if (this->m_StdArray->at(i)->StId == id) {
            index = i;
            break;
        }
    }
    return index;
}
void StudentManager::mod_Std() {
    if (this->m_fileIsEmpty) {
        cout << "文件不存在或者文件为空" << endl;
    }
    else {
        cout << "请输入需要修改的学生学号：" << endl;
        string id;
        cin >> id;
        int index = this->IsExist(id);
        if (index != -1) {
            //delete this -> m_StdArray -> at(index);
            string newid;
            string newname;
            string newdep;
            Student* std = NULL;
            cout << "搜索到学号为" << id << "的学生，请输入新学号：" << endl;
            cin >> newid;
            cout << "请输入新姓名：" << endl;
            cin >> newname;
            cout << "请输入新职责：" << endl;
            cin >> newdep;
            if (newdep == "班长") {
                std = new ClassPresident(newid, newname, newdep);
            }
            else if (newdep == "班干部") {
                std = new Classleader(newid, newname, newdep);
            }
            else if (newdep == "普通学生") {
                std = new Nomalstuden(newid, newname, newdep);
            }
            this->m_StdArray->at(index) = std;
            cout << "修改成功！" << endl;
            this->save();
        }
        else {
            cout << "修改失败，不存在该学号的学生！" << endl;
        }
    }
    system("pause");
}
void StudentManager::find_Std() {
    if (this->m_fileIsEmpty) {
        cout << "文件不存在或者文件为空" << endl;
    }
    else {
        cout << "请输入需要查找的学生学号：" << endl;
        string id;
        cin >> id;
        int index = this->IsExist(id);
        if (index != -1) {
            cout << "查找成功！该学生信息如下：" << endl;
            this->m_StdArray->at(index)->showInfo();
        }
        else {
            cout << "查找失败！该学生不存在！" << endl;
        }
    }
}
void StudentManager::clean_File() {
    cout << "确定清空所有数据？" << endl;
    cout << "1,确定" << endl;
    cout << "2,返回" << endl;
    int selet = 0;
    cin >> selet;
    if (selet == 1) {
        ofstream ofs(FILENAME, ios::trunc);
        ofs.close();
        if (this->m_StdArray) {
            this->m_StdArray->clear();
            vector<Student*>(*this->m_StdArray).swap(*this->m_StdArray);
            this->m_fileIsEmpty = true;
            this->m_StdArray = NULL;
        }
        cout << "清空成功！" << endl;
    }
    system("pause");
    system("cls");
}
StudentManager :: ~StudentManager() {
    if (this->m_StdArray) {
        this->m_StdArray->clear();
        delete[] this->m_StdArray;
        this->m_StdArray = NULL;
    }
}

//普通学生函数实现
Nomalstuden::Nomalstuden(string Id, string name, string dep) {
    this->StId = Id;
    this->m_name = name;
    this->m_Dept = dep;
}
void Nomalstuden::showInfo() {
    cout << "学生学号：" << this->StId
        << "\t学生姓名：" << this->m_name
        << "\t学生职位：" << this->m_Dept
        << "\t学生任务：遵守班级纪律" << endl;
}

//班长函数实现
ClassPresident::ClassPresident(string Id, string name, string dep) {
    this->StId = Id;
    this->m_name = name;
    this->m_Dept = dep;
}
void ClassPresident::showInfo() {
    cout << "学生学号：" << this->StId
        << "\t学生姓名：" << this->m_name
        << "\t学生职位：" << this->m_Dept
        << "\t学生任务：管理班干部，与辅导员对接，带领班级" << endl;
}

//班干部函数实现
Classleader::Classleader(string Id, string name, string dep) {
    this->StId = Id;
    this->m_name = name;
    this->m_Dept = dep;
}
void Classleader::showInfo() {
    cout << "学生学号：" << this->StId
        << "\t学生姓名：" << this->m_name
        << "\t学生职位：" << this->m_Dept
        << "\t学生任务：履行自己的职责，和各科老师对接，管理班级学生" << endl;
}

//主函数
int main() {
    StudentManager stm;     //实例化管理员
    int choice;             //存储用户选项
    while (true) {
        stm.Show_Menu();    //调用打印界面成员函数
        cout << "请输入您的选择：" << endl;
        cin >> choice;
        switch (choice)
        {
        case 0:             //退出系统
            stm.Exit_System();
            break;
        case 1:             //增加学生
            stm.Addinfo();
            break;
        case 2:             //显示学生
            stm.show_Std();
            break;
        case 3:             //删除学生
            stm.del_Std();
            break;
        case 4:             //修改学生
            stm.mod_Std();
            break;
        case 5:             //查找学生
            stm.find_Std();
            break;
        case 6:             //清空文档
            stm.clean_File();
            break;
        default:
            system("cls");  //清屏操作
            break;
        }
    }

    system("pause");
    return 0;
}
