#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <limits>
using namespace std;

class Admin;
class ExamService;

// 1. Abstract base class for all users
class User {
private:
    int userid;
    string password;
    string name;
    bool active;

    friend class Admin;

protected:
    User(int id, const string& n, const string& p)
        : userid(id), password(p), name(n), active(true) {}

public:
    virtual ~User() = default;

    int getUserId() const { return userid; }
    string getName() const { return name; }
    bool verifyPassword(const string& input) const {
        return password == input;
    }
    bool isActive() const { return active; }

    virtual string getRole() const = 0;
    virtual void displayDashboard() = 0;
};

// 2. Question class
class Question {
private:
    int questionId;
    string statement;
    vector<string> options;
    int correctOption;
    string explanation;
    double marks;

public:
    Question(int id, const string& text, const vector<string>& opts,
             int answer, const string& solution, double score)
        : questionId(id), statement(text), options(opts),
          correctOption(answer), explanation(solution), marks(score) {}

    int getQuestionId() const { return questionId; }
    string getStatement() const { return statement; }
    double getMarks() const { return marks; }

    void displayQuestion() const {
        cout << statement << '\n';
        for (size_t i = 0; i < options.size(); ++i) {
            cout << i + 1 << ". " << options[i] << '\n';
        }
    }

    bool checkAnswer(int answer) const {
        return answer == correctOption;
    }

    void setExplanation(const string& exp) { explanation = exp; }

    void showSolution() const {
        cout << "Explanation: " << explanation << '\n';
    }
};

// 3. Exam class
class Exam {
private:
    int examId;
    string title;
    int durationMinutes;
    vector<Question> questions;
    bool published;

public:
    Exam(int id, const string& examTitle, int duration)
        : examId(id), title(examTitle), durationMinutes(duration),
          published(false) {}

    int getExamId() const { return examId; }
    string getTitle() const { return title; }
    int getDuration() const { return durationMinutes; }
    bool isPublished() const { return published; }
    size_t getQuestionCount() const { return questions.size(); }

    void publish() {
        if (!questions.empty()) published = true;
    }

    bool addQuestion(const Question& question) {
        if (published) return false;
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
        if (published) return false;
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
        for (const Question& q : questions) total += q.getMarks();
        return total;
    }

    double conductExam() const {
        double score = 0;
        cout << "\nStarting exam: " << title << '\n';
        cout << "Duration: " << durationMinutes << " minutes\n";
        cout << "Total marks: " << getTotalMarks() << "\n\n";

        for (size_t i = 0; i < questions.size(); ++i) {
            int answer;
            cout << "Question " << i + 1 << ":\n";
            questions[i].displayQuestion();
            cout << "Enter option (1-4): ";

            while (!(cin >> answer) || answer < 1 || answer > 4) {
                cout << "Invalid input. Enter an option from 1 to 4: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (questions[i].checkAnswer(answer)) {
                score += questions[i].getMarks();
            }
            cout << '\n';
        }

        cout << "Exam submitted. Score: " << score
             << " / " << getTotalMarks() << '\n';
        return score;
    }

    void showSolutions() const {
        if (questions.empty()) {
            cout << "This exam has no questions.\n";
            return;
        }

        for (size_t i = 0; i < questions.size(); ++i) {
            cout << "\nQuestion " << i + 1 << ":\n";
            questions[i].displayQuestion();
            questions[i].showSolution();
        }
    }
};

// 4. Result class
class ExamResult {
private:
    int studentId;
    int examId;
    double obtainedMarks;
    double totalMarks;
    bool submitted;

public:
    ExamResult(int sid, int eid, double score, double total)
        : studentId(sid), examId(eid), obtainedMarks(score),
          totalMarks(total), submitted(true) {}

    int getStudentId() const { return studentId; }
    int getExamId() const { return examId; }
    double getScore() const { return obtainedMarks; }
    double getTotalMarks() const { return totalMarks; }
    bool isSubmitted() const { return submitted; }

    double getAccuracy() const {
        if (totalMarks <= 0) return 0.0;
        return obtainedMarks / totalMarks * 100.0;
    }

    bool hasPassed(double passPercentage = 40.0) const {
        return getAccuracy() >= passPercentage;
    }
};

// 5. Analytics helper
class Analytics {
public:
    static double averageMarks(const vector<ExamResult>& results) {
        if (results.empty()) return 0.0;
        double sum = 0;
        for (const auto& r : results) sum += r.getScore();
        return sum / results.size();
    }

    static double highestMarks(const vector<ExamResult>& results) {
        if (results.empty()) return 0.0;
        double highest = results[0].getScore();
        for (const auto& r : results) highest = max(highest, r.getScore());
        return highest;
    }

    static double lowestMarks(const vector<ExamResult>& results) {
        if (results.empty()) return 0.0;
        double lowest = results[0].getScore();
        for (const auto& r : results) lowest = min(lowest, r.getScore());
        return lowest;
    }

    static int countPassed(const vector<ExamResult>& results) {
        int count = 0;
        for (const auto& r : results) if (r.hasPassed()) ++count;
        return count;
    }

    static int countFailed(const vector<ExamResult>& results) {
        return static_cast<int>(results.size()) - countPassed(results);
    }
};

// 6. Central exam and result service
class ExamService {
private:
    vector<Exam> exams;
    vector<ExamResult> results;

public:
    bool addExam(const Exam& exam) {
        if (getExam(exam.getExamId()) != nullptr) return false;
        exams.push_back(exam);
        return true;
    }

    Exam* getExam(int examId) {
        for (auto& exam : exams) {
            if (exam.getExamId() == examId) return &exam;
        }
        return nullptr;
    }

    const Exam* getExam(int examId) const {
        for (const auto& exam : exams) {
            if (exam.getExamId() == examId) return &exam;
        }
        return nullptr;
    }

    const vector<Exam>& getAllExams() const { return exams; }
    const vector<ExamResult>& getAllResults() const { return results; }

    vector<ExamResult> getResultsForStudent(int studentId) const {
        vector<ExamResult> found;
        for (const auto& r : results) {
            if (r.getStudentId() == studentId) found.push_back(r);
        }
        return found;
    }

    vector<ExamResult> getResultsForExam(int examId) const {
        vector<ExamResult> found;
        for (const auto& r : results) {
            if (r.getExamId() == examId) found.push_back(r);
        }
        return found;
    }

    bool hasResult(int studentId, int examId) const {
        for (const auto& r : results) {
            if (r.getStudentId() == studentId && r.getExamId() == examId) {
                return true;
            }
        }
        return false;
    }

    bool saveResult(const ExamResult& result) {
        if (hasResult(result.getStudentId(), result.getExamId())) return false;
        results.push_back(result);
        return true;
    }
};

// 7. Student class
class Student : public User {
private:
    vector<int> attemptedExamIds;

public:
    Student(int id, const string& name, const string& pass)
        : User(id, name, pass) {}

    string getRole() const override { return "Student"; }

    void displayDashboard() override {
        cout << "\nStudent dashboard needs the exam service.\n";
    }

    void displayDashboard(ExamService& service) {
        int choice;
        do {
            cout << "\n--- Student Dashboard ---\n";
            cout << "Welcome, " << getName() << '\n';
            cout << "1. View available exams\n";
            cout << "2. Take an exam\n";
            cout << "3. View previous results\n";
            cout << "4. View solutions\n";
            cout << "5. View my rank\n";
            cout << "0. Logout\n";
            cout << "Choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }

            switch (choice) {
                case 1:
                    viewAvailableExams(service);
                    break;
                case 2: {
                    int examId;
                    cout << "Enter exam ID: ";
                    if (cin >> examId) takeTest(examId, service);
                    else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid exam ID.\n";
                    }
                    break;
                }
                case 3:
                    viewResults(service);
                    break;
                case 4: {
                    int examId;
                    cout << "Enter exam ID to view solutions: ";
                    if (cin >> examId) viewSolutions(examId, service);
                    else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid exam ID.\n";
                    }
                    break;
                }
                case 5:
                    viewRank(service);
                    break;
                case 0:
                    cout << "Logged out successfully.\n";
                    break;
                default:
                    cout << "Please enter a valid choice.\n";
            }
        } while (choice != 0);
    }

    void viewAvailableExams(const ExamService& service) const {
        bool found = false;
        cout << "\n--- Available Exams ---\n";
        for (const auto& exam : service.getAllExams()) {
            if (exam.isPublished()) {
                found = true;
                cout << "ID: " << exam.getExamId()
                     << " | Title: " << exam.getTitle()
                     << " | Duration: " << exam.getDuration() << " minutes"
                     << " | Questions: " << exam.getQuestionCount()
                     << " | Total marks: " << exam.getTotalMarks() << '\n';
            }
        }
        if (!found) cout << "No published exams are available.\n";
    }

    void takeTest(int examId, ExamService& service) {
        Exam* exam = service.getExam(examId);

        if (exam == nullptr || !exam->isPublished()) {
            cout << "Published exam not found.\n";
            return;
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
        vector<ExamResult> myResults = service.getResultsForStudent(getUserId());
        if (myResults.empty()) {
            cout << "You have not completed any exams yet.\n";
            return;
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

    void viewSolutions(int examId, const ExamService& service) const {
        const Exam* exam = service.getExam(examId);
        if (exam == nullptr || !exam->isPublished()) {
            cout << "Exam not found.\n";
            return;
        }
        if (!service.hasResult(getUserId(), examId)) {
            cout << "You can view solutions only after attempting this exam.\n";
            return;
        }
        exam->showSolutions();
    }

    void viewRank(const ExamService& service) const {
        vector<ExamResult> all = service.getAllResults();
        vector<ExamResult> mine = service.getResultsForStudent(getUserId());

        if (mine.empty()) {
            cout << "Take an exam first to get a rank.\n";
            return;
        }

        for (const auto& myResult : mine) {
            vector<ExamResult> sameExam = service.getResultsForExam(myResult.getExamId());
            sort(sameExam.begin(), sameExam.end(),
                 [](const ExamResult& a, const ExamResult& b) {
                     return a.getScore() > b.getScore();
                 });

            int rank = 1;
            for (const auto& r : sameExam) {
                if (r.getStudentId() == getUserId()) break;
                if (r.getScore() > myResult.getScore()) ++rank;
            }

            const Exam* exam = service.getExam(myResult.getExamId());
            cout << "Exam " << myResult.getExamId();
            if (exam != nullptr) cout << " (" << exam->getTitle() << ")";
            cout << ": Rank " << rank << " out of " << sameExam.size() << '\n';
        }
    }
};

// 8. Teacher class
class Teacher : public User {
public:
    Teacher(int id, const string& name, const string& pass)
        : User(id, name, pass) {}

    string getRole() const override { return "Teacher"; }

    void displayDashboard() override {
        cout << "\nTeacher dashboard needs the exam service.\n";
    }

    void displayDashboard(ExamService& service) {
        int choice;
        do {
            cout << "\n--- Teacher Dashboard ---\n";
            cout << "Welcome, " << getName() << '\n';
            cout << "1. Add question to an exam\n";
            cout << "2. Create exam\n";
            cout << "3. Add/update a solution\n";
            cout << "4. Publish exam\n";
            cout << "5. View student results\n";
            cout << "6. View class analytics\n";
            cout << "7. View student rankings\n";
            cout << "8. View my exams\n";
            cout << "0. Logout\n";
            cout << "Choice: ";

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
                case 0:
                    cout << "Logged out successfully.\n";
                    break;
                default:
                    cout << "Please enter a valid choice.\n";
            }
        } while (choice != 0);
    }

    void createExam(ExamService& service) {
        int examId, duration;
        string title;

        cout << "Enter exam ID: ";
        if (!(cin >> examId)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid exam ID.\n";
            return;
        }

        if (service.getExam(examId) != nullptr) {
            cout << "Exam ID already exists.\n";
            return;
        }

        cout << "Enter exam title: ";
        getline(cin >> ws, title);
        if (title.empty()) {
            cout << "Exam title cannot be empty.\n";
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

    void addQuestion(int examId, ExamService& service) {
        Exam* exam = service.getExam(examId);
        if (exam == nullptr) {
            cout << "Exam ID not found.\n";
            return;
        }
        if (exam->isPublished()) {
            cout << "Cannot add questions to a published exam.\n";
            return;
        }

        int questionId, correctOption;
        double marks;
        string statement, explanation;
        vector<string> options;

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

    void addSolution(int examId, int questionId, ExamService& service) {
        Exam* exam = service.getExam(examId);
        if (exam == nullptr) {
            cout << "Exam ID not found.\n";
            return;
        }
        if (exam->isPublished()) {
            cout << "Solutions cannot be changed after publishing.\n";
            return;
        }

        string explanation;
        cout << "Enter solution/explanation: ";
        getline(cin >> ws, explanation);
        if (explanation.empty()) {
            cout << "Explanation cannot be empty.\n";
            return;
        }

        if (exam->addSolution(questionId, explanation))
            cout << "Solution updated successfully.\n";
        else
            cout << "Question ID not found in this exam.\n";
    }

    void publishExam(int examId, ExamService& service) {
        Exam* exam = service.getExam(examId);
        if (exam == nullptr) {
            cout << "Exam ID not found.\n";
            return;
        }
        if (exam->isPublished()) {
            cout << "Exam is already published.\n";
            return;
        }
        if (exam->getQuestionCount() == 0) {
            cout << "Add at least one question before publishing.\n";
            return;
        }
        exam->publish();
        if (exam->isPublished()) cout << "Exam published successfully.\n";
        else cout << "Exam could not be published.\n";
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

    void viewStudentResults(const ExamService& service) const {
        int examId;
        cout << "Enter exam ID (or -1 for all results): ";
        if (!(cin >> examId)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid exam ID.\n";
            return;
        }

        bool found = false;
        for (const auto& r : service.getAllResults()) {
            if (examId != -1 && r.getExamId() != examId) continue;
            found = true;
            cout << "Student ID: " << r.getStudentId()
                 << " | Exam ID: " << r.getExamId()
                 << " | Score: " << r.getScore() << " / " << r.getTotalMarks()
                 << " | Percentage: " << r.getAccuracy() << "%"
                 << " | " << (r.hasPassed() ? "PASS" : "FAIL") << '\n';
        }
        if (!found) cout << "No results found for the selected exam.\n";
    }

    void viewClassAnalytics(const ExamService& service) const {
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
            if (examId == -1 || r.getExamId() == examId) selected.push_back(r);
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

    void viewStudentRankings(const ExamService& service) const {
        int examId;
        cout << "Enter exam ID for rankings: ";
        if (!(cin >> examId)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid exam ID.\n";
            return;
        }

        vector<ExamResult> selected = service.getResultsForExam(examId);
        if (selected.empty()) {
            cout << "No results available for this exam.\n";
            return;
        }

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

// 9. Admin class
class Admin {
public:
    void suspendAccount(User& user) { user.active = false; }
    void activateAccount(User& user) { user.active = true; }
};

// 10. Authentication service
class AuthService {
private:
    vector<unique_ptr<User>> users;

public:
    bool registerStudent(int id, const string& name, const string& pass) {
        if (id <= 0 || name.empty() || pass.empty()) return false;
        for (const auto& user : users) {
            if (user->getUserId() == id) return false;
        }
        users.push_back(make_unique<Student>(id, name, pass));
        return true;
    }

    bool registerTeacher(int id, const string& name, const string& pass) {
        if (id <= 0 || name.empty() || pass.empty()) return false;
        for (const auto& user : users) {
            if (user->getUserId() == id) return false;
        }
        users.push_back(make_unique<Teacher>(id, name, pass));
        return true;
    }

    User* login(int id, const string& pass) {
        for (const auto& user : users) {
            if (user->getUserId() == id &&
                user->verifyPassword(pass) && user->isActive()) {
                return user.get();
            }
        }
        return nullptr;
    }
};

// 11. Main system controller
class ExamSystem {
private:
    AuthService auth;
    Admin admin;
    ExamService examService;

public:
    void run() {
        int choice;
        do {
            cout << "\n===== ExamForge =====\n";
            cout << "1. Student signup\n";
            cout << "2. Login\n";
            cout << "0. Exit\n";
            cout << "Choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                continue;
            }

            if (choice == 1) {
                int id;
                string name, pass;

                cout << "Enter student ID: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                    continue;
                }

                cout << "Enter name: ";
                getline(cin >> ws, name);

                cout << "Enter password: ";
                cin >> pass;

                if (auth.registerStudent(id, name, pass))
                    cout << "Registration successful. You can now log in.\n";
                else
                    cout << "Registration failed. Check the ID and details.\n";
            } else if (choice == 3) {
                // Demo-only authorization code. Replace with secure admin
                // authentication before using this as a real online service.
                const string adminCode = "admin123";
                string enteredCode;
                cout << "Enter admin authorization code: ";
                cin >> enteredCode;

                if (enteredCode != adminCode) {
                    cout << "Unauthorized. Teacher registration denied.\\n";
                    continue;
                }

                int id;
                string name, pass;
                cout << "Enter new teacher ID: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\\n";
                    continue;
                }
                cout << "Enter teacher name: ";
                getline(cin >> ws, name);
                cout << "Enter teacher password: ";
                cin >> pass;

                if (auth.registerTeacher(id, name, pass))
                    cout << "Teacher registration successful.\\n";
                else
                    cout << "Teacher registration failed. Check the ID and details.\\n";
            } else if (choice == 2) {
                int id;
                string pass;

                cout << "Enter user ID: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID.\n";
                    continue;
                }

                cout << "Enter password: ";
                cin >> pass;

                User* user = auth.login(id, pass);
                if (user == nullptr) {
                    cout << "Invalid credentials or inactive account.\n";
                    continue;
                }

                cout << "Login successful: " << user->getRole() << '\n';

                if (auto* teacher = dynamic_cast<Teacher*>(user)) {
                    teacher->displayDashboard(examService);
                } else if (auto* student = dynamic_cast<Student*>(user)) {
                    student->displayDashboard(examService);
                } else {
                    user->displayDashboard();
                }
            } else if (choice != 0) {
                cout << "Please enter a valid choice.\n";
            }
        } while (choice != 0);

        cout << "Thank you for using ExamForge.\n";
    }
};

int main() {
    ExamSystem system;
    system.run();
    return 0;
}
