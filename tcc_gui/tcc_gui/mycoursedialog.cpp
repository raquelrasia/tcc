#include "mycoursedialog.h"
#include "ui_mycoursedialog.h"

/************* AUX METHOD *************/
// Gets and entry from a XML file given a root input, a tagname and an attribute.
// Ex.: tagname "Course" and attribute "Name" for course root.
QStringList getEntry(QDomElement root, QString tagname, QString attribute) {

    QStringList entries ;

    QDomNodeList items = root.elementsByTagName(tagname) ;

    for (int i = 0; i < items.count(); i++) {
        QDomNode itemnode = items.at(i) ;

        // convert to element
        if(itemnode.isElement()) {
            QDomElement itemele = itemnode.toElement() ;
            entries << itemele.attribute(attribute) ;
        }
    }

    return entries ;
}

/************* AUX METHOD *************/
// returns current course code
QString MyCourseDialog::get_current_course(){
    return CurrentCourse ;
}

/************* AUX METHOD *************/
// returns current class
QString MyCourseDialog::get_current_class(){
    return CurrentClass ;
}

/************* AUX METHOD *************/
// returns current semester
QString MyCourseDialog::get_current_semester(){
    return semester ;
}

/************* AUX METHOD *************/
// returns current year
QString MyCourseDialog::get_current_year(){
    return year ;
}

/************* AUX METHOD *************/
// returns state for current dialog.
bool MyCourseDialog::get_dialog_ended() {
    return dialog_ended ;
}

/************* AUX METHOD *************/
// returns path
QString MyCourseDialog::get_rec_dir() {
    return rec_dir ;
}

/************* AUX METHOD *************/
// returns the user path
QString MyCourseDialog::get_dir_videos(){
    return dir_videos ;
}

/************* AUX METHOD *************/
// returns the file name
QString MyCourseDialog::get_filename() {
    return filename ;
}

/************* AUX METHOD *************/
// Sets the directory mode. Autopath is enabled
// if mode is true and manual if mode is false
void MyCourseDialog::set_dir_mode(bool mode) {
    autopath = mode ;
}

/************* CONSTRUCTOR METHOD *************/
MyCourseDialog::MyCourseDialog(QWidget *parent, QString xml_path) :
    QDialog(parent),
    ui(new Ui::MyCourseDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Choose course and class");
    QString CourseNamesCode;
    // Uses Dom Model to open XML
    QDomDocument document ;
    QFile file(xml_path) ;

    // Performs some tests on the file
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to Open File" ;
    }
    else {
        if(!document.setContent(&file)) {
            qDebug() << "Failed to Load Document" ;
        }
        file.close();
    }

    // gets the root element
    root = document.firstChildElement() ;
    QDomElement teacher = root.firstChildElement() ;
    // gets the course_names for a given lecturer
    CourseList = getEntry(teacher, "course", "name") ;
    CourseIDList = getEntry(teacher, "course", "code") ;

    for(uint8_t i = 0; i < CourseList.size(); i++) {
        CourseNamesCode.append(CourseList.at(i));
        CourseNamesCode.append(" - ");
        CourseNamesCode.append(CourseIDList.at(i));
        ui->listWidget->addItem(CourseNamesCode);
        CourseNamesCode.clear();
    }

    // Selects the first course by default
    ui->listWidget->item(0)->setSelected(true);
    ui->listWidget_2->item(0)->setSelected(true);

    // Sets current row to 0. This is done due to a weird error that's been
    // happening with CurrentRow return value
    ui->listWidget->setCurrentRow(0);
    ui->listWidget_2->setCurrentRow(0);

    dialog_ended = false ;

    autopath = true ;

    QDateTime current_date = QDateTime::currentDateTime();
    filename = current_date.date().toString("dd_MM_yyyy");
}


/************* DESTRUCTOR METHOD *************/
MyCourseDialog::~MyCourseDialog()
{
    delete ui;
}

/************* LIST_WIDGET *************/
// Defines behaviour of the Course "LIST_WIDGET"
// This method is called everytime an item is selected
void MyCourseDialog::on_listWidget_itemSelectionChanged()
{
    ui->listWidget_2->clear();

    CurrentClassList.clear();

    QDomElement teacher = root.firstChildElement() ;
    // Class Selection Section
    QDomNodeList courses = teacher.elementsByTagName("course") ;
    // Solves a weird bug where currentRow() returns -1 (why, God?!)
    if(ui->listWidget->currentRow() == -1) {
        ui->listWidget->setCurrentRow(0) ;
    }
    // Gets the Code for a given class, e.g., S11, S71, S43, etc
    if(courses.at(ui->listWidget->currentRow()).isElement()) {
        ClassesNamesList << getEntry(courses.at(ui->listWidget->currentRow()).toElement(), "class", "name");
        YearsNamesList << getEntry(courses.at(ui->listWidget->currentRow()).toElement(), "class", "year");
        SemestersNamesList << getEntry(courses.at(ui->listWidget->currentRow()).toElement(), "class", "semester");
    }

    QString current_class;
    for(uint8_t i = 0; i < ClassesNamesList.size(); i++) {
       current_class.append(ClassesNamesList.at(i));
       current_class.append(" ");
       current_class.append(YearsNamesList.at(i));
       current_class.append("/");
       current_class.append(SemestersNamesList.at(i));
       qDebug() << current_class;
       CurrentClassList << current_class;
       current_class.clear();
    }
    

    // Adds the class code to the ListWidget
    for(uint8_t i = 0; i < CurrentClassList.size(); i++) {
        ui->listWidget_2->addItem(CurrentClassList.at(i));
    }

    // Sets CurrentCourse code and CurrentClass to the currently
    // selected on both widgets
    CurrentCourse = CourseIDList.at(ui->listWidget->currentRow()) ;

    ui->listWidget_2->setCurrentRow(0);
}

void MyCourseDialog::on_listWidget_2_itemSelectionChanged()
{
    // Solves a weird bug where currentRow() returns -1 (why, God?!)
    if(ui->listWidget_2->currentRow() == -1) {
        ui->listWidget_2->setCurrentRow(0);
    }
    CurrentClass = ClassesNamesList.at(ui->listWidget_2->currentRow()) ;
    semester = SemestersNamesList.at(ui->listWidget_2->currentRow());
    year = YearsNamesList.at(ui->listWidget_2->currentRow());
}

/************* PUSH_BUTTON *************/
// Defines behaviour of the "CONTINUE" push button
void MyCourseDialog::on_pushButton_2_clicked()
{
    if(CurrentCourse != "" && CurrentClass != "") {
        dialog_ended = close() ;
    }

    if(autopath == true) {
        dir_videos = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + QString("/") ;
        rec_dir = dir_videos ;

        QDir dir(rec_dir) ;

        // Creates a new folder for the recordings
        // This folder's name is the course code.
        // It doesn't matter that it actually doesn't
        // check whether the folder exists, for mkpath
        // only creates a directory if it doesn't already
        //exists
        dir.mkpath(rec_dir + QString("LectureRecordings/")) ;
        rec_dir = rec_dir + QString("LectureRecordings/") ;
        dir.mkpath(rec_dir + CurrentCourse) ;
        rec_dir = rec_dir + CurrentCourse + "/" ;
        dir.mkpath(rec_dir + CurrentClass) ;
        rec_dir = rec_dir + CurrentClass + "/" ;
        dir.mkpath(rec_dir +  year + "_" +semester) ;
        rec_dir = rec_dir + year+ "_" +semester + "/" ;
    }
}

/************* PUSH_BUTTON *************/
// Defines behaviour of the "BACK" push button
void MyCourseDialog::on_pushButton_clicked()
{
    close() ;
}
