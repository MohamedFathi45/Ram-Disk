#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<math.h>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <filesystem>
#include <experimental/filesystem>
#include"Text.h"
#include<map>
using namespace std;
struct DirectoryEntry;
struct DirectoryCluster;
struct DataCluster;
struct DataCluster {
    bool used;
    DataCluster* nxt;
    char data[30];         //caused error
    DataCluster() {
        nxt = NULL;
        used = 0;
    }
};
struct DirectoryEntrey {
    char name[300];
    unsigned long long size;
    DataCluster* starting_cluster;
    DirectoryCluster *next;
};
struct DirectoryCluster {
    vector<DirectoryEntrey> directory_entres;
    DirectoryCluster *prev;
};
DataCluster* arr = new DataCluster;
char* UseCharArray(string str ,int size) {
    char* arr = (char*)malloc(size * sizeof(char));
    strcpy(arr, str.c_str());
    return arr;
}
class FileSystem {
private:
    unsigned long long ids, emp, all_size;
    DirectoryCluster* root;     // we start from directory cluster root
    DirectoryCluster* cur;
    DirectoryEntrey* temp_holding_file;
    DirectoryCluster* source_folder;
    DirectoryCluster* distination_folder;
    DirectoryCluster* ori_src;
    DirectoryCluster* ori_dist;
    string folder_name;
    bool file_hold;
    bool folder_hold;
    bool cut_flag;
public:
    FileSystem() {
        source_folder = new DirectoryCluster;
        distination_folder = new DirectoryCluster;
        temp_holding_file = NULL;
        source_folder = NULL;
        distination_folder = NULL;
        file_hold = 0;
        folder_hold = 0;
        ids = 0;
        cut_flag = 0;
        folder_name = "";
        cur = root;
    }
    string get_folder_name() {
        return folder_name;
    }
    string get_file_system(char* path) {
        string p = path;
        for (int i = 0; i < p.size(); i++) {
            if (p[i] == ':')
                return p.substr(0, i);
        }
        return "N";
    }
    void link(DirectoryCluster*& parent, DirectoryCluster*& child, char* name) {    //link two folders
        DirectoryEntrey* t = new DirectoryEntrey;
        strcpy(t->name, name);
        t->starting_cluster = NULL;
        t->next = child;
        parent->directory_entres.push_back(*t);
        child->prev = parent;
    }
    bool is_folder(string str) {
        char* n = UseCharArray(str, 300);
        for (int i = 0; i < cur->directory_entres.size(); i++) {
            if (strcmp(n, cur->directory_entres[i].name) == 0) {
                if (cur->directory_entres[i].starting_cluster == NULL)
                    return 1;
                return 0;
            }
        }
        return 0;
    }
    bool is_folder(string str, DirectoryCluster* current) {
        char* n = (UseCharArray(str,300));
        for (int i = 0; i < current->directory_entres.size(); i++) {
            if (strcmp(n, current->directory_entres[i].name) == 0) {
                if (current->directory_entres[i].starting_cluster == NULL)
                    return 1;
                return 0;
            }
        }
    }
    void move_to_loc(char* name, DirectoryCluster*& current) {
        bool ok = 0;
        for (int i = 0; i < current->directory_entres.size(); i++) {
            if (strcmp(current->directory_entres[i].name, name) == 0) {
                current = current->directory_entres[i].next;
                ok = 1;
                break;
            }
        }
        if (!ok)
            cout << " Directory NotFound" << endl;
    }
    void creat_folder(char* f_name, DirectoryCluster*& p) {
        DirectoryEntrey* t1 = new DirectoryEntrey;
        DirectoryCluster* temp = new DirectoryCluster;
 
        temp->prev = p;
        strcpy(t1->name, f_name);       t1->starting_cluster = NULL;            t1->next = temp;
        p->directory_entres.push_back(*t1);
    }
    void creat_folder(char* f_name) {
        DirectoryEntrey* t1 = new DirectoryEntrey;
        DirectoryCluster* temp = new DirectoryCluster;
        temp->prev = cur;
        strcpy(t1->name, f_name);       t1->starting_cluster = NULL;            t1->next = temp;
        cur->directory_entres.push_back(*t1);
    }
    bool move_to(string f_name) {
        bool f = 0;
        for (int i = 0; i < cur->directory_entres.size(); i++) {
            if (cur->directory_entres[i].name == f_name) {
                if (cur->directory_entres[i].next != NULL) {
                    cur = cur->directory_entres[i].next;
                    f = 1;
                    break;
                }
            }
        }
        if (!f)
            return 0;
        return 1;
    }
    bool is_path(char* path) {
        string str = path;
        for (int i = 0; i < str.size(); i++) {
            if (str[i] == '\\')
                return 1;
        }
        if (strcmp(path, "Root") == 0)
            return 1;
        return 0;
    }
    void print_path(DirectoryCluster* p) {
 
    }
    void print_path() {
    }
    void move_back() {
        if (cur == root) {
            cout << "Cannot move back" << endl;
        }
        else
            cur = cur->prev;
    }
    void move_back(DirectoryCluster*& p) {
        p = p->prev;
    }
    void print_dir() {
        for (int i = 0; (unsigned long long) i < cur->directory_entres.size(); i++) {
            if (cur->directory_entres[i].starting_cluster != NULL)
                cout << cur->directory_entres[i].name << " " << "\t" << cur->directory_entres[i].size / (1024 * 1024) << "MB" << endl;
            else
                cout << cur->directory_entres[i].name << endl;
        }
 
    }
    DataCluster* internal_read(DataCluster* s, unsigned long long& size) {
        DataCluster* start = new DataCluster;
        start = s;
        DataCluster* first_cluster = new DataCluster;
        DataCluster* prev_cluster = new DataCluster;
        unsigned long long i;
        if (emp >= ceil(size / 30)) {
            bool first = 1;
            for (i = 0; i <= all_size && start != NULL; i++) {
                if (first && arr[i].used == 0) {
                    emp--;
                    memcpy(arr[i].data, start->data, sizeof(arr[i].data));
                    first = 0;
                    first_cluster = prev_cluster = &arr[i];
                    arr[i].nxt = NULL;
                    start = start->nxt;
                    arr[i].used = 1;
                }
                else if (arr[i].used == 0 && !first) {
                    emp--;
                    memcpy(arr[i].data, start->data, sizeof(arr[i].data));
                    prev_cluster->nxt = &arr[i];
                    arr[i].nxt = NULL;
                    prev_cluster = &arr[i];
                    arr[i].used = 1;
                    start = start->nxt;
                }
            }
            return first_cluster;
        }
        else {
            cout << "No size" << endl;
            return NULL;
        }
    }
    DataCluster* read(char* path, unsigned long long& size) {
        //char buff[30];
        FILE* file = fopen(path, "rb");
        if (!file) {
            cout << "Falid" << endl;
            return NULL;
        }
        fseek(file, 0, SEEK_END);     //move to the end of file
        unsigned long long n = ftell(file);            // the current position of file
        size = n;
        fseek(file, 0, SEEK_SET);
        bool first_pos = 1;
        int sz;
        DataCluster* first_adress = new DataCluster;
        DataCluster* prev_block = new DataCluster;
        unsigned long long i;
        if (emp >= ceil(n / 30)) {
            for (i = 0; i < all_size; i++) {
                if (first_pos && arr[i].used == 0) {
                    if ((sz = fread(arr[i].data, 1, sizeof(arr[i].data), file)) != 0) {
                        first_adress = prev_block = &arr[i];
                        arr[i].nxt = NULL;
                        first_pos = 0;
                        arr[i].used = 1;
                        emp--;
                    }
                    else {
                        break;
                    }
                }
                else {
                    if (arr[i].used == 0 && !first_pos) {
                        if ((sz = fread(arr[i].data, 1, sizeof(arr[i].data), file)) != 0) {
                            arr[i].nxt = NULL;
                            prev_block->nxt = &arr[i];
                            prev_block = &arr[i];
                            emp--;
                            arr[i].used = 1;
                        }
                        else {
                            break;
                        }
                    }
                }
            }
            fclose(file);
            return first_adress;
        }
        else {
            fclose(file);
            cout << "No Space" << endl;
            return NULL;
        }
    }
    void write(DataCluster* start, char* path) {
        FILE* file = fopen(path, "wb+");
        if (file == NULL) {
            cout << "Error opening the file" << endl;
            return;
        }
        DataCluster* temp = start;
        while (start != NULL) {
            fwrite(start->data, 1, sizeof(start->data), file);
            start = start->nxt;
        }
        fclose(file);
    }
    char* get_name(string name) {
        int ind = 0;
        for (int i = name.size() - 1; i >= 0; i--) {
            if (name[i] == '\\') {
                ind = i;
                break;
            }
        }
        string ret = name.substr(ind + 1, (name.size() - ind + 1));
        char * r = UseCharArray(ret, 300);
        return r;
    }
    void copy_helper(char* name) {
        for (auto &entry : std::experimental::filesystem::directory_iterator(name)) {
            string dir = entry.path().string();
            struct stat s;
            char* str = UseCharArray(dir, 300);
            if (stat(str, &s) == 0) {           //directory
                if (s.st_mode & S_IFDIR) {
                    char* n = get_name(dir);
                    creat_folder(n);
                    move_to(n);
                    copy_helper(str);
                    move_back();
                }
                else if (s.st_mode & S_IFREG) {         //file
                    DirectoryEntrey *t = new DirectoryEntrey;
                    DataCluster* temp = read(str, t->size);
                    if (temp == NULL) {
                        delete t;
                        return;
                    }
                    t->starting_cluster = temp;     strcpy(t->name, get_name(str));
                    t->next = NULL;
                    cur->directory_entres.push_back(*t);
                }
            }
            else {
                cout << "Error" << endl;
            }
        }
    }
 
    void internal_copy(char* path) {
        if (!is_path(path)) {
            if (is_folder(path)) {
                folder_name = path;
                folder_hold = 1;        file_hold = 0;
                bool ok = 0;
                for (int i = 0; i < cur->directory_entres.size(); i++) {
                    if (strcmp(cur->directory_entres[i].name, path) == 0) {
                        source_folder = new DirectoryCluster(*cur->directory_entres[i].next);
                        ori_src = cur->directory_entres[i].next;
                        ok = 1;
                        break;
                    }
                }
                if (!ok) {
                    cout << " Direcotry NotFound" << endl;
                }
                return;
            }
            else {
                file_hold = 1;      folder_hold = 0;
                bool ok = 0;
                for (int i = 0; i < cur->directory_entres.size(); i++) {
                    if (strcmp(cur->directory_entres[i].name, path) == 0) {
                        temp_holding_file = new DirectoryEntrey(cur->directory_entres[i]);
                        ori_src = cur;
                        ok = 1;
                        break;
                    }
                }
                if (!ok) {
                    cout << "File not found" << endl;
                }
                return;
            }
        }
        string t = path;
        DirectoryCluster*temp = new DirectoryCluster( *root );
        DirectoryCluster*previous = temp;
        vector<string>road = Text::split_text(t);
        int j = 0;
        for (int i = 1; i < road.size(); i++) {
            bool ok = 0;
            for (j = 0; j < temp->directory_entres.size(); j++) {
                if (temp->directory_entres[j].name == road[i]) {
                    previous = temp;
                    temp = temp->directory_entres[j].next;
                    ok = 1;
                    break;
                }
            }
            if (!ok) {
                cout << "Wrong path" << endl;
                return;
            }
        }
        if (temp != NULL) {         //folder
            source_folder = new DirectoryCluster(*temp);    folder_name = get_name(path);   //if temp changes source_folder changes too
            ori_src = temp;
 
            folder_hold = 1;        file_hold = 0;
        }
        else {
            file_hold = 1;      folder_hold = 0;
            source_folder = new DirectoryCluster(*previous);
            ori_src = previous;
            temp_holding_file = new DirectoryEntrey(previous->directory_entres[j]);
        }
    }
 
    void copy(char* path) {
        if (get_file_system(path) == "Root" || !is_path(path)) {
            internal_copy(path);
            return;
        }
        struct stat s;
        if (stat(path, &s) == 0) {
            if (s.st_mode & S_IFDIR) {      //directory
                char* n = get_name(path);
                creat_folder(n);
                move_to(n);
                copy_helper(path);
                move_back();
            }
            else if (s.st_mode & S_IFREG) {     //file
                DirectoryEntrey* t = new DirectoryEntrey;
                DataCluster* temp = read(path, t->size);
                if (temp == NULL) {
                    return;
                }
                t->starting_cluster = temp;
                strcpy(t->name, get_name(path));
                t->next = NULL;
                cur->directory_entres.push_back(*t);            //cased by null value (solved)
            }
        }
        else {
            cout << "Error" << endl;
        }
    }
    void past_helper(string path) {
        for (int i = 0; i < cur->directory_entres.size(); i++) {
            string temp = path;
            path += "\\";           path += cur->directory_entres[i].name;
            if (cur->directory_entres[i].starting_cluster == NULL) {            //direcotry
                std::experimental::filesystem::create_directory(path);
                move_to(cur->directory_entres[i].name);
                past_helper(path);
                move_back();
            }
            else {                                                  //file
                char* str = UseCharArray(path,300);
                write(cur->directory_entres[i].starting_cluster, str);
            }
            path = temp;
        }
    }
    void internal_past_helper() {
        for (int i = 0; i < source_folder->directory_entres.size(); i++) {
            if (source_folder->directory_entres[i].starting_cluster == NULL) {              //folder
                creat_folder(source_folder->directory_entres[i].name, distination_folder);
                move_to_loc(source_folder->directory_entres[i].name, distination_folder);
                move_to_loc(source_folder->directory_entres[i].name, source_folder);
                internal_past_helper();
                move_back(source_folder);
                move_back(distination_folder);
            }
            else {                                                      //file
                DirectoryEntrey* t = new DirectoryEntrey(source_folder->directory_entres[i]);           //problem in this block of code
                t->starting_cluster = internal_read(t->starting_cluster, t->size);
                if (t->starting_cluster == NULL) {
                    return;
                }
                t->next = NULL;
                distination_folder->directory_entres.push_back(*t);
            }
        }
    }
    void internal_past(char* path) {
        bool single_file_flag = 0;
        if (!is_path(path) || strlen(path) == 0) {
            single_file_flag = 1;
        }
        if (!single_file_flag) {
            vector<string>road = Text::split_text(path);
            DirectoryCluster* temp = root;
            for (int i = 1; i < road.size(); i++) {
                bool ok = 0;
                for (int j = 0; j < temp->directory_entres.size(); j++) {
                    if (temp->directory_entres[j].name == road[i]) {
                        ok = 1;
                        temp = temp->directory_entres[j].next;
                        break;
                    }
                }
                if (!ok) {
                    cout << "Wrong path" << endl;
                    return;
                }
            }
            distination_folder = new DirectoryCluster(*temp);
            ori_dist = temp;
        }
        else {
            distination_folder = new DirectoryCluster(*cur);
            ori_dist = cur;
        }
        if (folder_hold) {
            if (source_folder == NULL || distination_folder == NULL) {
                cout << "No source or No distination , try again" << endl;
                return;
            }
        }
        if (file_hold) {
            if (temp_holding_file == NULL || distination_folder == NULL) {
                cout << "Nothing to past" << endl;
                return;
            }
        }
        if (folder_hold) {
            //cout << distination_folder << endl;
            char* str = UseCharArray(folder_name, 300);
            creat_folder(str, distination_folder);
            move_to_loc(str, distination_folder);
            for (int i = 0; i < source_folder->directory_entres.size(); i++) {
                if (source_folder->directory_entres[i].starting_cluster == NULL) {      //folder
                    creat_folder(source_folder->directory_entres[i].name, distination_folder);
                    move_to_loc(source_folder->directory_entres[i].name, distination_folder);
                    move_to_loc(source_folder->directory_entres[i].name, source_folder);
                    internal_past_helper();
                    move_back(source_folder);
                    move_back(distination_folder);
                }
                else {                                                              //file
                    DirectoryEntrey* t = new DirectoryEntrey(source_folder->directory_entres[i]);           //problem in this block of code
                    t->starting_cluster = internal_read(t->starting_cluster, t->size);
                    if (t->starting_cluster == NULL) {
                        return;
                    }
                    t->next = NULL;
                    distination_folder->directory_entres.push_back(*t);
                }
            }
 
            link(ori_dist, distination_folder, str);
        }
        else {
            DirectoryEntrey* t;
            t = new DirectoryEntrey(*temp_holding_file);
            t->starting_cluster = internal_read(t->starting_cluster, t->size);
            ori_dist->directory_entres.push_back(*t);
        }
        if (cut_flag) {
            cut_flag = 0;
            if (folder_hold) {
                ori_src = ori_src->prev;
                char* str = UseCharArray(folder_name, 300);
                bool ok = 0;
                for (int i = 0; i < ori_src->directory_entres.size(); i++) {
                    if (strcmp(str, ori_src->directory_entres[i].name) == 0) {
                        del(&ori_src->directory_entres[i], ori_src, i);
                        ok = 1;
                        break;
                    }
                }
                if (ok) {
                    source_folder = NULL;
                    distination_folder = NULL;
                }
                else
                    cout << "Directory Not Found" << endl;
            }
            else {
                bool ok = 0;
                for (int i = 0; i < ori_src->directory_entres.size(); i++) {
                    if (strcmp(temp_holding_file->name, ori_src->directory_entres[i].name) == 0) {
                        del(&ori_src->directory_entres[i], ori_src, i);
                        ok = 1;
                        break;
                    }
                }
                if (ok) {
                    temp_holding_file = NULL;
                    source_folder = NULL;
                    distination_folder = NULL;
                }
                else
                    cout << "File Not Found" << endl;
            }
        }
    }
 
    void test() {
        for (int i = 0; i < source_folder->directory_entres.size(); i++) {
            cout << source_folder->directory_entres[i].name << endl;
            if (source_folder->directory_entres[i].starting_cluster == NULL) {
                source_folder = source_folder->directory_entres[i].next;
                test();
                source_folder = source_folder->prev;
            }
        }
    }
 
    void past(string path) {
        char* str = UseCharArray(path, 200);
        if (get_file_system(str) == "Root") {
            internal_past(str);
            return;
        }
        else {
            past_helper(path);
        }
    }
    void del_helper(DirectoryCluster* d) {
        for (int i = 0; i < d->directory_entres.size(); i++) {
            if (d->directory_entres[i].starting_cluster != NULL) {      //file
                DataCluster* temp = d->directory_entres[i].starting_cluster;
                while (temp != NULL && temp->used == 1) {
                    emp++;
                    temp->used = 0;
                    temp = temp->nxt;
                }
            }
            else {                                  //directory
                move_to_loc(d->directory_entres[i].name, d);
                del_helper(d);
                move_back(d);
            }
        }
        d = NULL;
    }
    void del(DirectoryEntrey* d, DirectoryCluster* cur_dir, int loc) {
        DataCluster* temp = d->starting_cluster;
        if (temp == NULL) {                 //folder ==> iterate over all the folders and files inside
            del_helper(cur_dir->directory_entres[loc].next);
        }
        else {
            while (temp != NULL && temp->used == 1) {
                emp++;
                temp->used = 0;
                temp = temp->nxt;
            }
        }
        cur_dir->directory_entres.erase(cur_dir->directory_entres.begin() + loc);
    }
    void pastFto(char* name, char* path) {
        if (is_folder(name)) {
            DirectoryCluster* t = cur;
            bool ok = 0;
            for (int i = 0; i < cur->directory_entres.size(); i++) {
                if (strcmp(name, cur->directory_entres[i].name) == 0) {
                    cur = cur->directory_entres[i].next;
                    ok = 1;
                    break;
                }
            }
            if (ok) {
                past(path);
                cur = t;
            }
            else {
                cout << " Not found" << endl;
                return;
            }
        }
        else {
            for (int i = 0; i < cur->directory_entres.size(); i++) {
                if (strcmp(cur->directory_entres[i].name, name) == 0) {
                    string p = path;
                    p += "\\";
                    p += name;
                    char* str = UseCharArray(p, 300);
                    write(cur->directory_entres[i].starting_cluster, str);
                    return;
                }
            }
        }
    }
    void cut(char* name) {
        cut_flag = 1;
        copy(name);
    }
    DirectoryCluster* get_cur_location() {
        return cur;
    }
    unsigned long long get_EmptySpots() {
        return emp;
    }
    void show_size() {
        cout << floor((emp * 30) / (1024 * 1024)) << "MB" << endl;
    }
    void init(unsigned long long k) {
        emp = ceil((k * 1024 * (unsigned long long)1024) / 30);
        all_size = emp;
        arr = new DataCluster[emp];
 
        root = new DirectoryCluster;
        cur = root;     // this is suppose to change the value of the cur pointer
        root->prev = NULL;
    }
    void help() {
        cout << "copy\t copy file or folder from spicific location on the hard disk or the file system\t Ex (copy D:\\Folder)" << endl;
        cout << "list\t listing all the folders and files inside the curretn directory\t Ex(list)" << endl;
        cout << "past\t paste the current directory to spicific location on the hard disk\t Ex(past D:\\Folder)" << endl;
        cout << "pastf\t paste file or folder that is already copied or cut from the file system itself\t Ex(pastf)" << endl;
        cout << "mov\t move from the current directory to anothe direcotry in the file system\t Ex(mov folder)" << endl;
        cout << "back\t move back to the previos directory \t Ex(back)" << endl;
        cout << "del\t delete spicific folder or file from the file system\t Ex(del folder)" << endl;
        cout << "cf\t creat folder in the current directory\t Ex(cf Folder)" << endl;
        cout << "pastFto\t paste folder or file form file system to spicific location on the hard disk\t Ex(pastFto Folder|D:\\Newfolder )" << endl;
        cout << "size\t show the free size of the file system \t Ex(size)" << endl;
        cout << "cut\t cut folder or file from system \t Ex (cut Folder)" << endl;
    }
 
};
string prev(string str) {
    for (int i = str.size() - 1; i >= 0; i--) {
        if (str[i] == '\\')
            return str.substr(0, i);
    }
    return str;
}
 
int main() {
    FileSystem F1;
    string str;
    char path[200];
    unsigned long long l;
    cout << "Size in MB:";
    cin >> l;
    F1.init(l);
    string cur = "Root:";
    while (true) {
        cout << cur << ">";
        cin >> str;
        if (str == "copy") {
            cin.ignore();
            cin.getline(path, 200);
            F1.copy(path);
        }
        else if (str == "past") {
            cin.ignore();
            cin.getline(path, 200);
            F1.past(path);
        }
        else if (str == "pastf") {
            F1.internal_past("");
        }
        else if (str == "list") {
            F1.print_dir();
        }
        else if (str == "mov") {
            cin.ignore();
            cin.getline(path, 200);
            if (F1.move_to(path) == 0) {
                cout << "Directory not found" << endl;
            }
            else {
                cur += "\\";    cur += path;
            }
        }
        else if (str == "back") {
            F1.move_back();
            cur = prev(cur);
        }
        else if (str == "del") {
            cin.ignore();
            cin.getline(path, 200);
            bool ok = 0;
            DirectoryCluster* temp = F1.get_cur_location();
            for (int i = 0; i < temp->directory_entres.size(); i++) {
                if (strcmp(path, temp->directory_entres[i].name) == 0) {
                    F1.del(&temp->directory_entres[i], temp, i);
                    ok = 1;
                    break;
                }
            }
            if (!ok)
                cout << "File not found" << endl;
        }
        else if (str == "cf") {
            cin.ignore();
            cin.getline(path, 200);
            F1.creat_folder(path);
        }
        else if (str == "pastFto") {
            char name[200];
            cin.ignore();
            cin.getline(name, 200,'|');
            cin.getline(path, 200);
            F1.pastFto(name, path);
        }
        else if (str == "size") {
            F1.show_size();
        }
        else if (str == "cut") {
            cin.ignore();
            cin.getline(path, 200);
            F1.cut(path);
        }
        else if (str == "help") {
            F1.help();
        }
    }
}
