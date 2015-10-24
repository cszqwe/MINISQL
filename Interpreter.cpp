#include "Interpreter.h"
//#include "API.h"

void apicaller::wronginput(){      //输入错误，重新输入并开始判断。
    cout<<"Your SQL query is wrong, please input again!"<<endl;
    comm="";
    commtype=-1;
    queryin();
}
void apicaller::queryin(){      //输入query SQL 然后直接开始执行判断
    cin>>comm;
    judgecommtype();

    //cout<<comm<<endl;

}
void apicaller::judgecommtype(){//create table:6, create index:7, drop table:8, drop index:9, select:1, insert:2, delete:3, quit:4, execfile:5, error:-1
    if (comm=="select") {
        cin>>comm;
        
        commtype=1;
        callselect();
    }
    if (comm=="insert") {
        cin>>comm;
        
        commtype=2;
        callinsert();
    }
    if (comm=="delete") {
        cin>>comm;
        
        commtype=3;
        calldelete();
    }
    if (comm=="quit;") {
        commtype=4;
        exit(0);
        
    }
    if (comm=="execfile") {             //没有对应函数
        cin>>comm;
        commtype=5;
        
    }
    if (comm=="create") {
        cin>>comm;
        
        if (comm=="table") {
            commtype=6;
            callcreatetable();
        }
        if (comm=="index") {
            commtype=7;
            callcreateindex();
        }
    }
    if (comm=="drop") {
        cin>>comm;
        
        if (comm=="table") {
            commtype=8;
            calldroptable();
        }
        if (comm=="index") {
            commtype=9;
            calldropindex();
        }
    }
    if (commtype==-1) {
        cin.sync();
        cin.clear();
        wronginput();
    }
    
}
void apicaller::callselect(){               //未完成
    /*select * from student;
    select * from student where sno = ‘88888888’;
    select * from student where sage > 20 and sgender = ‘F’;
     */
    string tableName;               //必须先给出该table对应的各个属性的类型及其顺序
    int opNum;
    OpType op[100];
    
    cin>>comm;
    if (comm!="*") {
        commtype=-1;
        return;
    }
    cin>>comm;
    if (comm!="from") {
        commtype=-1;
        return;
    }
    cin>>tableName;
    cin>>comm;
    if (comm!="where") {
        commtype=-1;
        return;
    }
                                        //op，未写完
    
}
void apicaller::callinsert(){   //insert into student values (‘12345678’,’wy’,22,’M’);
    string tableName;
    int attrNum;
    string attr[32];
    cin>>comm;
    if (comm=="into") {
        cin>>tableName;                         //必须先给出该table对应的各个属性的类型及其顺序
        int i=1;
        cin>>comm;
        if (comm=="values") {
            cin>>comm;                          //保存以上例子中(‘12345678’,’wy’,22,’M’);
                                                //继续处理，未完成
        }
        else commtype=-1;
    } else {
        commtype=-1;
        return;
    }
    
}
void apicaller::calldelete(){   //delete from student; //delete from student where sno = ‘88888888’;
    string tableName;
    bool haveOp;
    OpType op;
    cin>>comm;
    if (comm=="from") {
        cin>>comm;
        if (comm.find(";")!=string::npos) {
            haveOp=false;
                                //此处调用函数
        }
        else {
            haveOp=true;
            cin>>comm;
            if (comm=="where") {
                cin>>op.attrName;
                cin>>comm;
                if (comm=="<")
                    op.ope=1;
                if (comm==">")
                    op.ope=2;
                if (comm=="<=")
                    op.ope=3;
                if (comm==">=")
                    op.ope=4;
                if (comm=="=")
                    op.ope=5;
                if (comm=="<>")
                    op.ope=6;
                cin>>comm;
                
                                                            //缺少attrType判断函数
                op.value=comm.substr(comm.find("'")+1,comm.find("';")-1);
                op.value=comm.substr(0,comm.find(";")-1);
                //cout<<op.value;
                                                            //以及attrPos调用函数
                                                            //然后调用api.delete()函数
            }
            
        }
    }
    else commtype=-1;
    

}
void apicaller::callcreatetable(){
    /*
     create table student ( sno char(8), sname char(16) unique, sage int, sgender char(1), primary key ( sno ) );
     */
    string tableName;
    int attrNum;
    string attrName[32];
    int attrType[32];
    
    cin>>tableName;
    cin>>comm;
    if (comm=="(") {        //此处类型？
        int i=0;
        cin>>comm;
        while (comm!="primary"&&comm!=");"&&i<=31) {
            attrName[i]=comm;
            cin>>comm;
            if (comm.find("char")!=string::npos) {
                attrType[i]=3;
            }
            if (comm.find("int")!=string::npos)
                attrType[i]=1;
            if (comm.find("float")!=string::npos)
                attrType[i]=2;
            if (comm.find(',')==string::npos) {
                cin>>comm;
                if (comm.find("unique")!=string::npos) {
                    attrType[i] +=20;
                }
            } else {
            }
            cin>>comm;
            i++;
        }
        attrNum=i;
        if (comm=="primary") {
            cin>>comm;              //"key"
            cin>>comm;              //"("
            cin>>comm;              //table name
            for (int i=0; i<attrNum; i++) {
                if (attrName[i] == comm) {
                    attrType[i] += 10;
                }
            }
        }
        while (comm.find(";")==string::npos) {
            cin>>comm;
        }
        
    } else {
        commtype=-1;
        return;
    }
                    //调用API
    //cout<<tableName<<attrNum<<" "<<attrName[0]<<" "<<attrName[1]<<" "<<attrName[2]<<" "<<attrName[3]<<" "<<attrType[0]<<" "<<attrType[1]<<" "<<attrType[2]<<" "<<attrType[3]<<endl;
}
void apicaller::callcreateindex(){   //create index stunameidx on student ( sname );
    string tableName;
    string indexName;
    string attrName;
    
    cin>>indexName;
    cin>>comm;
    if (comm=="on") {
        cin>>tableName;
        cin>>comm;
        if (comm=="(") {
            cin>>attrName;
            cin>>comm;
            if (comm==");"){}
            else {
                commtype=-1;
            }
        } else {
            commtype=-1;
        }
    } else {
        commtype=-1;
    }
                    //调用API
    //cout<<indexName<<endl<<tableName<<endl<<attrName<<endl;
}
void apicaller::calldroptable(){   //drop table student;
    string tableName;
    cin>>tableName;
    if((tableName.find(';')!=string::npos)&&((tableName.find(';')+1)==tableName.length())){
    tableName=tableName.substr(0,tableName.find(';'));
    }
    else {
        commtype=-1;
    }
    //调用API
    //cout<<tableName<<endl;
}
void apicaller::calldropindex(){   //drop index stunameidx;
    string indexName;
    cin>>indexName;
    if((indexName.find(';')!=string::npos)&&((indexName.find(';')+1)==indexName.length())){
        indexName=indexName.substr(0,indexName.find(';'));
    }
    else {
        commtype=-1;
    }
    //调用API
}

int main(int argc, const char * argv[]) {
    
    cout<<"SQL query： "<<endl;
    apicaller work;
    work.queryin();
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
