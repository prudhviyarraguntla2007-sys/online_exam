#include<iostream>
#include<string> 
#include<vector>
#include<memory>
#include<algorithm>
#include<numeric>
#include<cstdio>
#include<limits>
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
    int getDuration() const {
    return durationMinutes;
    }
    bool isPublished() const {
        return published;
    }
    size_t getQuestionCount() const {
        return questions.size();
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

    double conductExam() const {
        // TODO: Implement timed examination and submission.
        double score=0;
        cout<<"\nStarting exam: "<<title<<'\n';
        cout<<" Duration : "<<durationMinutes<<" miniutes\n";
        cout<<"Total marks: "<<getTotalMarks()<<"\n\n";
        for(int  i=0;i<questions.size();i++){
            cout<<"Question  "<<i+1<<endl;
            questions[i].displayQuestion();
            cout << "Please enter the option: (1-4) ";
            int answer;
           while (!(cin >> answer) || answer < 1 || answer > 4) {
                cout << "Invalid input. Enter an option from 1 to 4: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (questions[i].checkAnswer(answer)) {
                score += questions[i].getMarks();
            }
        }
        cout<<"Exam submitted. "<<endl;
        cout << "Score: " << score << " / " << getTotalMarks() << '\n';
        return score;
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
        if(getExam(exam.getExamId())!=nullptr)
        return false;
        exams.push_back(exam);
        return true;
    }

    // Find an exam using its ID
  // For modifying an exam
Exam* getExam(int examId) {
    for (auto& exam : exams) {
        if (exam.getExamId() == examId) {
            return &exam;
        }
    }
    return nullptr;
}

// For reading an exam through a const service
const Exam* getExam(int examId) const {
    for (const auto& exam : exams) {
        if (exam.getExamId() == examId) {
            return &exam;
        }
    }
    return nullptr;
}
    //getting all the exams 
    const vector<Exam>& getAllExams() const{
        return exams;
    }
    const vector<ExamResult>& getAllResults() const {
        return results;
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
    void displayDashboard() override{
        cout<<"\n Student dashboard needs the exam service  as the argument.\n";
    }
    void displayDashboard(ExamService& service){
        int choice;
        do{
            cout << "\n--- Student Dashboard ---\n";
        cout << "Welcome, " << getName() << '\n';
        cout << "1. View available exams\n";
        cout << "2. Take an exam\n";
        cout << "3. View previous results\n";
        cout << "4. View solutions\n";
        cout << "5. View my rank\n";
        cout << "6.Logout\n";
        cout << "Please Enter your choice ";
        if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }
            
        switch(choice){
            case 1:
                viewAvailableExams(service);
                break;
            case 2:{
                    int examid;
                cout<<"Enter the Exam Id : ";
                if(cin>>examid)
                takeTest(examid,service);
                else{
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cout<<"Invalid Exam Id ."<<endl;
                }
                break;
            }
            case 3:
                viewResults(service);
                break;
            case 4:{
                int examid;
                cout<<"Enter the Exam Id : ";
                if(cin>>examid)
                viewSolutions(examid,service);
                else{
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cout<<"Invalid Exam Id ."<<endl;
                }
                break;
            }
            case 5:
                viewRank(service);
                break;
            case 6:
                cout<<"Logged out sucsessfully.\n";
                break;
            default:
                cout<<"Please Enter the valid input "<<endl;
        }       
        }while(choice!=6);      
    }
    void viewAvailableExams(const ExamService& service) const{
        bool found =false;
        cout<<"\n-----Available Exams --\n";
        for(const auto& exam :service.getAllExams()){
            if(exam.isPublished()){
                found =true;
                 cout << "ID: " << exam.getExamId()
                     << " | Title: " << exam.getTitle()
                     << " | Duration: " << exam.getDuration() << " minutes"
                     << " | Questions: " << exam.getQuestionCount()
                     << " | Total marks: " << exam.getTotalMarks() << '\n';

            }
        }
        if(!found)
        cout<<"no published eams are available .\n";
    }
    void takeTest(int examId, ExamService& service)  {
            Exam* exam=service.getExam(examId);
            if(exam==nullptr||!exam->isPublished()){
                cout<<"no Exams are present" <<endl;
                cout<<"Published Exam is not found "<<endl;
            }
             if (service.hasResult(getUserId(), examId)) {
            cout << "You have already attempted this exam.\n";
            return;
        }

        double score = exam->conductExam();
        ExamResult result(getUserId(), examId, score, exam->getTotalMarks());

        if (service.saveResult(result)) {
            attemptedExamIds.push_back(examId);
            cout << "Result saved successfully.\n";
        } else {
            cout << "Could not save result: a result already exists.\n";
        }
    }


     void viewResults(const ExamService& service) const {
        vector<ExamResult> myResults=service.getResultsForStudent(getUserId());
        if(myResults.empty()){
            cout<<" You have not comleted any exms yet."<<endl;
            return ;
        }
         cout << "\n--- Previous Results ---\n";
        for (const auto& result : myResults) {
            const Exam* exam = service.getExam(result.getExamId());
            cout << "Exam ID: " << result.getExamId();
            if (exam != nullptr) cout << " | " << exam->getTitle();
            cout << " | Score: " << result.getScore() << " / "
                 << result.getTotalMarks()
                 << " | Percentage: " << result.getAccuracy() << "%"
                 << " | " << (result.hasPassed() ? "PASS" : "FAIL") << '\n';
        }
    }
      void viewSolutions(int examId,const ExamService& service) const {
       const Exam* exam=service.getExam(examId);
       if(exam==nullptr||!exam->isPublished()){
        cout<<"Exma is not found."<<endl;
        return ;
       }
       if(!service.hasResult(getUserId(),examId)){
        cout<<"You can view the solutions only after the exams"<<endl;
        return;
        }
        exam->showSolution();
    }
     void viewRank(const ExamService& service) const {
    vector<ExamResult> mine =
        service.getResultsForStudent(getUserId());

    if (mine.empty()) {
        cout << "Take an exam first to get a rank.\n";
        return;
    }

    for (const auto& myResult : mine) {
        vector<ExamResult> sameExam =
            service.getResultsForExam(myResult.getExamId());

        sort(sameExam.begin(), sameExam.end(),
             [](const ExamResult& a, const ExamResult& b) {
                 return a.getScore() > b.getScore();
             });

        int rank = 1;

        for (const auto& r : sameExam) {
            if (r.getStudentId() == getUserId()) break;

            if (r.getScore() > myResult.getScore()) {
                ++rank;
            }
        }

        const Exam* exam =
            service.getExam(myResult.getExamId());

        cout << "Exam " << myResult.getExamId();

        if (exam != nullptr) {
            cout << " (" << exam->getTitle() << ")";
        }

        cout << ": Rank " << rank
             << " out of " << sameExam.size() << '\n';
    }
}
    };
//8. teacher class
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
        cout << "Publish Exam\n";
        cout << "5. View student results\n";
        cout << "6. View class analytics\n";
        cout << "7. View student rankings\n";
        cout << "8.view my exams\n";
        cout << "9.To exit ot logout ";
        cout<<"Enter your valid choice ";
        if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }

            switch (choice) {
                case 1: {
                    int examId;
                    cout << "Enter exam ID: ";
                    if (cin >> examId) addQuestion(examId, service);
                    else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid exam ID.\n";
                    }
                    break;
                }
                case 2:
                    createExam(service);
                    break;
                case 3: {
                    int examId, questionId;
                    cout << "Enter exam ID: ";
                    if (!(cin >> examId)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid exam ID.\n";
                        break;
                    }
                    cout << "Enter question ID: ";
                    if (!(cin >> questionId)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid question ID.\n";
                        break;
                    }
                    addSolution(examId, questionId, service);
                    break;
                }
                case 4: {
                    int examId;
                    cout << "Enter exam ID to publish: ";
                    if (cin >> examId) publishExam(examId, service);
                    else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid exam ID.\n";
                    }
                    break;
                }
                case 5:
                    viewStudentResults(service);
                    break;
                case 6:
                    viewClassAnalytics(service);
                    break;
                case 7:
                    viewStudentRankings(service);
                    break;
                case 8:
                    viewMyExams(service);
                    break;
                case 9:
                    cout << "Logged out successfully.\n";
                    break;
                default:
                    cout << "Please enter a valid choice.\n";
            }
        } while (choice != 9);
    }
   void createExam(ExamService& service){
    int examId,duration;
    string title;
    cout<<"Enter  Exam Id: ";
    if(!(cin>>examId)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<< "Invalid exam Id.\n";
        return ;
    }
    if(service.getExam(examId)!=nullptr){
        cout<<"Already Exam is present .\n";
        return;
    }
    cout<<"Enter exam title : ";
    getline(cin>> ws,title);
    if(title.empty()){
        cout<<"Exam title cannot be empty.\n";
        return;
    }
     cout << "Enter duration in minutes: ";
        if (!(cin >> duration) || duration <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Duration must be a positive number.\n";
            return;
        }

        Exam exam(examId, title, duration);
        if (service.addExam(exam)) cout << "Exam created successfully.\n";
        else cout << "Could not create exam. Check the ID.\n";
   }
    void addQuestion(int examId,ExamService& service) {
        Exam* exam=service.getExam(examId);
        if(exam==nullptr){
            cout<<"No Exam is present on this Id \n";
            return;
        }
        if(exam->isPublished()){
            cout<<"Cannot add the question at this time \n";
            return ;
        }
    int questionId;
    string statement;
    vector<string> options;
    int correctOption;
    int marks;
    string explanation;
    cout << "Enter question ID: ";
        if (!(cin >> questionId)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid question ID.\n";
            return;
        }

        if (exam->hasQuestionId(questionId)) {
            cout << "Question ID already exists in this exam.\n";
            return;
        }

        cout << "Enter question statement: ";
        getline(cin >> ws, statement);
        if (statement.empty()) {
            cout << "Question statement cannot be empty.\n";
            return;
        }
        if (exam->hasQuestionStatement(statement)) {
            cout << "This question already exists in this exam.\n";
            return;
        }

        for (int i = 0; i < 4; ++i) {
            string option;
            cout << "Enter option " << i + 1 << ": ";
            getline(cin >> ws, option);
            if (option.empty()) {
                cout << "Options cannot be empty. Question was not added.\n";
                return;
            }
            options.push_back(option);
        }

        cout << "Enter correct option (1-4): ";
        if (!(cin >> correctOption) || correctOption < 1 || correctOption > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid correct option. Question was not added.\n";
            return;
        }

        cout << "Enter marks: ";
        if (!(cin >> marks) || marks <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Marks must be greater than zero. Question was not added.\n";
            return;
        }

        cout << "Enter solution/explanation: ";
        getline(cin >> ws, explanation);

        Question question(questionId, statement, options, correctOption,
                          explanation, marks);
        if (exam->addQuestion(question)) cout << "Question added successfully.\n";
        else cout << "Could not add question.\n";
}

  void addSolution(int examId, int questionId,ExamService& service) {
        Exam* exam=service.getExam(examId);
        if(exam==nullptr){
            cout<<"Exam id is not found .\n";
            return ;
        }
        if(exam->isPublished()){
            cout<<"You cannot add the solution to the published exam \n";
            return ;
        }
        string explanation;
        cout << "Enter solution/explanation: ";
        getline(cin >> ws, explanation);

        if (explanation.empty()) {
        cout << "Explanation cannot be empty.\n";
        return;
}
        if(exam->addSolution(questionId,explanation)){
            cout<<"Solution added sucessfully ";

        }
        else 
        cout<<"Question Id not found in this exam \n";
    }
    void publishExam(int examId,ExamService& service){
        Exam* exam=service.getExam(examId);
        if(exam==nullptr){
            cout<<"No Exam exists in this id \n";
            return ;
        }
        if(exam->isPublished()){
            cout<<"This exam is already published.\n";
            return;
        }
        if(exam->getQuestionCount()==0){
            cout<<"No questions present in the exam.\n";
            return;
        }
       exam->publish();

        if (exam->isPublished()) {
        cout << "Exam published successfully.\n";
        } else {
        cout << "Exam could not be published.\n";
            }
    }
     void viewMyExams(const ExamService& service) const {
        bool found = false;
        cout << "\n--- Exams ---\n";
        for (const auto& exam : service.getAllExams()) {
            found = true;
            cout << "ID: " << exam.getExamId()
                 << " | Title: " << exam.getTitle()
                 << " | Duration: " << exam.getDuration() << " minutes"
                 << " | Questions: " << exam.getQuestionCount()
                 << " | Total marks: " << exam.getTotalMarks()
                 << " | Status: " << (exam.isPublished() ? "Published" : "Draft")
                 << '\n';
        }
        if (!found) cout << "No exams have been created.\n";
    }
    // I think there is small pending here
    //because of the presence of -1
    //so let's see this later 
    void viewStudentResults(const ExamService& service) const{
        int examId;
        cout<<"Enter exam Id (or -1 for all results ): \n";
        if(!(cin>>examId)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Invalid Exam Id .\n";
            return ;
        }
        bool found =false;
        for(const auto& r:service.getAllResults()){
            if(examId!=-1&&r.getExamId()!=examId)continue;
            found=true;
                        cout << "Student ID: " << r.getStudentId()
                 << " | Exam ID: " << r.getExamId()
                 << " | Score: " << r.getScore() << " / " << r.getTotalMarks()
                 << " | Percentage: " << r.getAccuracy() << "%"
                 << " | " << (r.hasPassed() ? "PASS" : "FAIL") << '\n';
        }
        if (!found) cout << "No results found for the selected exam.\n";
   
        }
    
    void viewClassAnalytics(ExamService& service) const {
    int examId;
        cout << "Enter exam ID for analytics (or -1 for all results): ";
        if (!(cin >> examId)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid exam ID.\n";
            return;
        }

        vector<ExamResult> selected;
        for (const auto& r : service.getAllResults()) {
            if (examId == -1 || r.getExamId() == examId)
             selected.push_back(r);
        }

        if (selected.empty()) {
            cout << "No results available for analytics.\n";
            return;
        }

        cout << "\n--- Class Analytics ---\n";
        cout << "Number of submissions: " << selected.size() << '\n';
        cout << "Average marks: " << Analytics::averageMarks(selected) << '\n';
        cout << "Highest marks: " << Analytics::highestMarks(selected) << '\n';
        cout << "Lowest marks: " << Analytics::lowestMarks(selected) << '\n';
        cout << "Passed: " << Analytics::countPassed(selected) << '\n';
        cout << "Failed: " << Analytics::countFailed(selected) << '\n';
    }
    //Ranknings of the students 
    void viewStudentRankings(ExamService& service) const {
        int examId;
        cout<<"Enter exam Id for rankings: ";
        if(!(cin>>examId)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Invalid Exam \n";
            return;     
    }
    vector<ExamResult> selected = service.getResultsForExam(examId);
        if (selected.empty()) {
            cout << "No results available for this exam.\n";
            return;
        }
        // syntax for sorting of the marks
        // selecting the exam and going into it 
        //getting compare the scores then
          sort(selected.begin(), selected.end(),
             [](const ExamResult& a, const ExamResult& b) {
                 if (a.getScore() != b.getScore()) return a.getScore() > b.getScore();
                 return a.getStudentId() < b.getStudentId();
             });
        cout << "\n--- Student Rankings ---\n";
        int rank = 1;
        for (const auto& r : selected) {
            cout << "Rank " << rank++
                 << " | Student ID: " << r.getStudentId()
                 << " | Score: " << r.getScore() << " / " << r.getTotalMarks()
                 << " | Percentage: " << r.getAccuracy() << "%\n";
        }
    }
};


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