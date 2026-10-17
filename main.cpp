#include<iostream>
#include<string> 
#include<vector>
#include<memory>
#include<algorithm>
#include<numeric>
#include<cstdio>
using namespace std;

class Admin;
// Defining the user class 
// using the abstract base class 

class User{
private:
    int userid;
    string password;
    string  name;
    bool active;
    //admin need to get the access of the user class need to modiufy whatever he wants
    friend class Admin;
    // here constructor is called 
protected:
        //based on the research this is not such efficient function to call constructors and functions
        //the second and written appraoch is better because the the intialzed whn there is const reference passing 

        // User(int id,const string& n,const string& p){
        //     userid=id;
        //     password=p;
        //     active=true;
        //     name=n;
        // }
        //so this is the better approch then the above one 
        User(int id,const string& n,const string& p):
        userid(id),password(p),name(n),active(true){}
public:
        virtual ~User()=default;
        int getUserId() const{
            return userid;
        }
        string getName() const{
            return name;
        }
        bool    verifyPassword(const string& input) const {
            return password==input;
        }
        bool isActive() const {
            return active;
        }
        //getting the details or get the overview of the students
        virtual string getRole() const =0;
        
        virtual void displayDashboard() =0;

    //then we have to ask for the sign in and sign up for the user then need to validate the detail according to it 
    //
};
//2.questions 
// class of the questions and opotions and answers 
// with options and soltions 
class Question {
private:
    int questionId;
    string statement;
    vector<string> options;
    int correctOption;
    string explanation;
    double marks;

public:
    Question(int id, const string& text,
             const vector<string>& opts,
             int answer, const string& solution,
             double score)
        : questionId(id), statement(text),
          options(opts), correctOption(answer),
          explanation(solution), marks(score) {}

    int getQuestionId() const {
        return questionId;
    }

    string getStatement() const{
        return statement;
    }

    double getMarks() const {
        return marks;
    }

    void displayQuestion() const {
        cout << statement << '\n';

        for (size_t i = 0; i < options.size(); i++) {
            cout << i + 1 << ". " << options[i] << '\n';
        }
    }

    bool checkAnswer(int answer) const {
        return answer == correctOption;
    }
    void setExplanation(const string& exp) {
            explanation = exp;
    }
    void showSolution() const {
        cout << "Explanation: " << explanation << '\n';
    }
};
// 3.Exam class
// creating the exam
//creating a exam which now includes the time and the set of questions and time in it 
// which exam  means to does not overlap the exam schedule
class Exam {
private:
    int examId;
    string title;
    int durationMinutes;
    vector<Question> questions;
    bool published;

public:
    Exam(int id, const string& examTitle, int duration)
        : examId(id), title(examTitle),
          durationMinutes(duration), published(false) {}

    int getExamId() const {
        return examId;
    }

    string getTitle() const {
        return title;
    }

    bool isPublished() const {
        return published;
    }
    size_t getQuestionCount() const {
        return questoins.size();
    }
    void publish() {
        if(!questions.empty())published=true;
    }

    bool addQuestion(const Question& question) {
        if (published) {
            return false;
        }
        questions.push_back(question);
        return true;
    }

      bool hasQuestionId(int id) const {
        for (const Question& q : questions) {
            if (q.getQuestionId() == id) return true;
        }
        return false;
    }

    bool hasQuestionStatement(const string& text) const {
        for (const Question& q : questions) {
            if (q.getStatement() == text) return true;
        }
        return false;
    }

    bool addSolution(int questionId, const string& explanation) {
        if(published)return false;
        for (Question& q : questions) {
            if (q.getQuestionId() == questionId) {
                q.setExplanation(explanation);
                return true;
            }
        }
        return false;
    }

    double getTotalMarks() const {
        double total = 0;

        for (const Question& q : questions) {
            total += q.getMarks();
        }

        return total;
    }

    void conductExam() const {
        // TODO: Implement timed examination and submission.
        double score=0;
        cout<<"\nStarting exam: "<<title<<'\n';
        cout<<" Duration : "<<durationMinutes<<" miniutes\n";
        cout<<"Total marks: "<<getTotalMarks()<<"\n\n";
        for(int  i=0;i<questions.size();i++){
            int ans;
            cout<<"Question  "<<i+1<<endl;
            questions[i].displayQuestion();
            cout << "Please enter the option: (1-4) ";
           while (!(cin >> answer) || answer < 1 || answer > 4) {
                cout << "Invalid input. Enter an option from 1 to 4: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (questions[i].checkAnswer(ans)) {
                score += questions[i].getMarks();
            }
        }
        cout<<"Exam submitted. "<<endl;
        cout << "Score: " << score << " \ " << getTotalMarks() << '\n';
    }
    void showSolution() const{
        if(questions.empty()){
            cout<<"This exam has no questions.\n";
            return;
        }
        for(int i=0;i<questions.size();i++){
            cout<<"\n Question  "<<i+1<<" :\n";
            questions[i].displayQuestion();
            questions[i].showSolution();
        }
    }
};
//4.Result  class 
// get the marks of the each student 
// and min max marks of the class and required updates 
class ExamResult {
private:
    int studentId;
    int examId;
    double obtainedMarks;
    double totalMarks;
    bool submitted;

public:
    ExamResult(int sid, int eid, double score, double total)
        : studentId(sid), examId(eid),
          obtainedMarks(score), totalMarks(total),
          submitted(true) {}

    int getStudentId() const {
        return studentId;
    }

    int getExamId() const {
        return examId;
    }

    double getScore() const {
        return obtainedMarks;
    }

    double getTotalMarks() const {
        return totalMarks;
    }

    bool isSubmitted() const {
        return submitted;
    }

    double getAccuracy() const {
        if (totalMarks <= 0) return 0.0;

        return (obtainedMarks / totalMarks) * 100.0;
    }

    bool hasPassed(double passPercentage = 40.0) const {
        return getAccuracy() >= passPercentage;
    }
};
//5.ananlytics class
// analysis of the  class and understanding level of the students and difficult of paper
// my new plan but optional like according to average of the paper i can get the difficulty level of the paper and get the result 
class Analytics {
public:
    static double averageMarks(
        const vector<ExamResult>& results) {

        if (results.empty()) return 0.0;

        double sum = 0;

        for (const ExamResult& r : results) {
            sum += r.getScore();
        }

        return sum / results.size();
    }

    static double highestMarks(
        const vector<ExamResult>& results) {

        if (results.empty()) return 0.0;

        double highest = results[0].getScore();

        for (const ExamResult& r : results) {
            highest = max(highest, r.getScore());
        }

        return highest;
    }

    static double lowestMarks(
        const vector<ExamResult>& results) {

        if (results.empty()) return 0.0;

        double lowest = results[0].getScore();

        for (const ExamResult& r : results) {
            lowest = min(lowest, r.getScore());
        }

        return lowest;
    }

    static int countPassed(
        const vector<ExamResult>& results) {

        int count = 0;

        for (const ExamResult& r : results) {
            if (r.hasPassed()) {
                count++;
            }
        }

        return count;
    }

    static int countFailed(
        const vector<ExamResult>& results) {

        return static_cast<int>(results.size())
               - countPassed(results);
    }
};

//6. central Exam and result sservice
//6.Exam service class 
//11.class Exam services
// to manage the exam and the result and to do not overlap between the system 
// to make the more structred 
class ExamService {
private:
    vector<Exam> exams;
    vector<ExamResult> results;

public:
    // Store a newly created exam
    bool addExam(const Exam& exam) {
        if(getexam(exam.getExamId())!=nullptr)
        return false;
        exams.push_back(exam);
        return true;
    }

    // Find an exam using its ID
    Exam* getExam(int examId) {
        for (auto& exam : exams) {
            if (exam.getExamId() == examId) {
                return &exam;
            }
        }
        return nullptr;
    }
    //getting all the exams 
    const vector<Exam>& getallExams() const{
        return exams;
    }
    const vector<ExamResult>& getAllResults() canst {
        return result;
    }
    //get student data on the result 
    vector<ExamResult> getResultsForStudent(int studentId) const{
        vector<ExamResult> found;
        for(const auto& r :results){
            if(r.getStudentId()==studentId)
            found.push_back(r);
        }
        return found;
    }
   vector<ExamResult> getResultsForExam(int examId) const{
    vector<ExamResult> found;
    for(const auto& r:results){
        if(r.getExamId()==examId)
        found.push_back(r);
    }
    return found;

   }
   bool hasResult(int studentId,int examId) const{
    for(const auto& r: results){
        if(r.getStudentId()==studentId&&r.getExamId()==examId)
        return true;
    }
    return false;
   }
   //saving the data of the all students
      bool saveResult(const ExamResult& result) {
        if (hasResult(result.getStudentId(), result.getExamId())) return false;
        results.push_back(result);
        return true;
    }
};

//7.student
class Student: public User{
private:
    vector<int> attemptedExamIds;
public:
    Student(int id,const string& name,const string& pass):User(id,name,pass){

    }
    string getRole() const override{
        return "Student";
    }
    void displayDashboard()  override{
        int choice;
        cout << "\n--- Student Dashboard ---\n";
        cout << "Welcome, " << getName() << '\n';
        cout << "1. View available exams\n";
        cout << "2. Take an exam\n";
        cout << "3. View previous results\n";
        cout << "4. View solutions\n";
        cout << "5. View my rank\n";
        cout<<"Please Enter your choice ";
        cin>>choice;
    
    }
      void takeTest(int examId, ExamService& service);  
     void viewResults() const {
        // TODO: Retrieve this student's exam results.
    }
      void viewSolutions(int examId) const {
        // TODO: Show solutions when permitted.
    }
     void viewRank() const {
        // TODO: Retrieve the student's rank.
    }
};
//3. teacher class 
class  Teacher: public User{
    private:
    vector<Exam> createdExamIds;

public:
    Teacher(int id, const string& name, const string& pass)
        : User(id, name, pass) {}

    string getRole() const override {
        return "Teacher";
    }
    //using the fuction overloding 
    void displayDashboard() override {
    cout << "\nTeacher dashboard requires ExamService.\n";
}
    void displayDashboard(ExamService& service)   {//1
          int choice;
        do{
          
        
        cout << "\n--- Teacher Dashboard ---\n";
        cout << "Welcome, " << getName() << '\n';
        cout << "1. Create questions\n";
        cout << "2. Create and publish exams\n";
        cout << "3. Add solutions\n";
        cout << "4. View student results\n";
        cout << "5. View class analytics\n";
        cout << "6. View student rankings\n";
        cout << "7.To exit ot logout ";
        cout<<"Enter your valid choice ";
        cin>>choice;
        if(choice==1){
            int examid;
            cout<<"Enter the exam id ";
            cin>>examid;
            addQuestion(examid);
        }
        else if(choice==2){
                createExam(service);
        }
        else if(choice==3){
            int questionId;
            cout<<" Enter the question Id\t";
            cin>>questionId;
            int examid;
            cout<<"Enter the ExamId  ";
            cin>>examid;
            addSolution(examid,questionId);
        }
        else if(choice==4){
            
        }
        else if(choice==5){
            viewClassAnalytics();
        }
        else if(choice==6){
            viewStudentRankings();
        }
        else if(choice==7){
            cout<<"You are log out succesfully "<<endl;
        }
        else{
            cout<<"please Enter a valid choice from the above "<<endl;
        }
        }while(choice!=7);
        

    }
   void createExam(ExamService& service);

    void addQuestion(int examId) {
    int questionId;
    string statement;
    vector<string> options;
    int correctOption;
    int marks;
    string explanation;
    for (auto& exam : createdExamIds) {
        if (exam.getExamId() == examId) {
            if (exam.isPublished()) {
                cout << "Cannot add questions to a published exam.\n";
                return;
            }

            cout << "Enter Question ID: ";
            cin >> questionId;

            // NEW: Check for duplicate question ID
            if (exam.hasQuestionId(questionId)) {
                cout << "Question ID already exists in this exam.\n";
                return;
            }
            cout << "Enter question statement: ";
            getline(cin >> ws, statement);

            // NEW: Check for duplicate question statement
            if (exam.hasQuestionStatement(statement)) {
                cout << "This question already exists in this exam.\n";
                return;
            }
            for (int i = 0; i < 4; i++) {
                string option;
                cout << "Enter option " << i + 1 << ": ";
                getline(cin >> ws, option);
                options.push_back(option);
            }

            cout << "Enter correct option (1-4): ";
            cin >> correctOption;

            if (correctOption < 1 || correctOption > 4) {
                cout << "Invalid correct option.\n";
                return;
            }

            cout << "Enter marks: ";
            cin >> marks;

            if (marks <= 0) {
                cout << "Marks must be greater than zero.\n";
                return;
            }

            cout << "Enter solution/explanation: ";
            getline(cin >> ws, explanation);

            Question newQuestion(
                questionId,
                statement,
                options,
                correctOption,
                explanation,
                marks
            );

            // Keep the original approach
            exam.addQuestion(newQuestion);

            cout << "Question added successfully!\n";
            return;
        }
    }

    cout << "Exam ID not Found.\n";
}

  void addSolution(int examId, int questionId) {
        for (auto& exam : createdExamIds) {
            if (exam.getExamId() == examId) {
                string explanation;
                cout << "Enter solution/explanation: ";
                getline(cin >> ws, explanation);
                if (exam.addSolution(questionId, explanation)) {
                    cout << "Solution updated successfully.\n";
                } else {
                    cout << "Question ID not found in this exam.\n";
                }
                return;
            }
        }
        cout << "Exam ID not found.\n";
    }

    void viewClassAnalytics() const {
        // TODO: Calculate class performance.
    }

    void viewStudentRankings() const {
        // TODO: Display ranked exam results.
    }
};


void Student::takeTest(int examId, ExamService& service) {
    Exam* exam = service.getExam(examId);

    if (exam == nullptr) {
        cout << "Exam not found.\n";
        return;
    }

    attemptedExamIds.push_back(examId);

    exam->conductExam();
}
void Teacher::createExam(ExamService& service) {
    int examId, duration;
    string title;

    cout << "Enter Exam ID: ";
    cin >> examId;

    if (service.getExam(examId) != nullptr) {
        cout << "Exam ID already exists.\n";
        return;
    }

    cout << "Enter exam title: ";
    getline(cin >> ws, title);

    cout << "Enter duration in minutes: ";
    cin >> duration;

    if (duration <= 0) {
        cout << "Duration must be greater than zero.\n";
        return;
    }

    Exam exam(examId, title, duration);
    service.addExam(exam);

    cout << "Exam created successfully.\n";
}

//8. admin class 
//now giving the power to the admin such he can get the acess of the every student 
// Every thing like gettnig the password of ths each student and anlaysis of the each one
//using this class we can conclude that we can give the acess to the student and 
// then only students cannot get the aces of the setting and seeing the paper before the exam 
class Admin {
public:
    void suspendAccount(User& user) {
        user.active = false;
    }

    void activateAccount(User& user) {
        user.active = true;
    }
};
// 9.authentication service 
// by this only we can go to the each class and give the aces to the each student 
// in this class only you can register the new teacher with thier permission 
// assigning happens from  here only 
class AuthService {
private:
    vector<unique_ptr<User>> users;

public:
    bool registerStudent(
        int id, const string& name, const string& pass) {

        for (const auto& user : users) {
            if (user->getUserId() == id) {
                return false;
            }
        }

        users.push_back(
            make_unique<Student>(id, name, pass)
        );

        return true;
    }

    bool registerTeacher(
        int id, const string& name, const string& pass) {

        // TODO: Restrict teacher registration to authorized admins.
        for (const auto& user : users) {
            if (user->getUserId() == id) {
                return false;
            }
        }

        users.push_back(
            make_unique<Teacher>(id, name, pass)
        );

        return true;
    }

    User* login(int id, const string& pass) {
        for (const auto& user : users) {
            if (user->getUserId() == id &&
                user->verifyPassword(pass) &&
                user->isActive()) {
                return user.get();
            }
        }

        return nullptr;
    }
};
//10. connecting class 
// till  they are many class the gate and  oreder of all the above classes is defined in this class
// this is the only function that calls in the main function 
// this is only class to decide the flow from one to one or through this only we can go to the above class 
class ExamSystem {
private:
    AuthService auth;
    Admin admin;
    ExamService examService;

public:
        // registering and login happening sucesfully
    void run() {
        int choice;
        do {
            cout << "\n===== Exam System =====\n";
            cout << "1. Student signup\n";
            cout << "2. Login\n";
            cout << "0. Exit\n";
            cout << "Choice: ";
            cin >> choice;
            if (choice == 1) {
                int id;
                string name, pass;

                cout << "Enter student ID: ";
                cin >> id;

                cout << "Enter name: ";
                cin >> ws;
                getline(cin, name);

                cout << "Enter password: ";
                cin >> pass;

                if (auth.registerStudent(id, name, pass)) {
                    cout << "Registration successful.\n";
                } else {
                    cout << "User ID already exists.\n";
                }
            }
            else if (choice == 2) {
                int id;
                string pass;

                cout<< "Enter user ID: ";
                cin >> id;

                cout << "Enter password: ";
                cin >> pass;

                User* user = auth.login(id, pass);

                if (user != nullptr) {
                    cout << "Login successful: "
                         << user->getRole() << '\n';

                    // Runtime polymorphism:
                   if (auto* teacher =dynamic_cast<Teacher*>(user)) {
                                teacher->displayDashboard(examService);
                    }
                    else {
                                user->displayDashboard();
                    }
                } else {
                    cout << "Invalid credentials or inactive account.\n";
                }
            }

        } while (choice != 0);
    }
};
// when the function is runnning then it is auotmatically call the class and enters in to the class

int main() {
    ExamSystem system;
    system.run();

    return 0;
}




// int main(){

//     // int userid;
//     // string password;
//     // cout<<"Enter the userID :";
//     // cin>>userid;
//     // cout<<"Enter your Password :";
//     // cin>>password;
    
//     return 0;
// }