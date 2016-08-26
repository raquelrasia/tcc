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
// returns current course
QString MyCourseDialog::get_current_course(){
    return CurrentCourse ;
}

/************* AUX METHOD *************/
// returns current class
QString MyCourseDialog::get_current_class(){
    return CurrentClass ;
}

/************* AUX METHOD *************/
// returns state for current dialog.
bool MyCourseDialog::get_dialog_ended() {
    return dialog_ended ;
}

MyCourseDialog::MyCourseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyCourseDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Choose course and class");

    // Uses Dom Model to open XML
    QDomDocument document ;

    /******************* PEGAR .XML DO SITE *******************/
    /******************* PEGAR .XML DO SITE *******************/
    QFile file("C:/Users/luisribeiro/Documents/tcc/tcc_teste/tcc_teste/courses.xml") ;
    /******************* PEGAR .XML DO SITE *******************/
    /******************* PEGAR .XML DO SITE *******************/

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

    // gets the course_names for a given lecturer
    CourseList = getEntry(root, "Course", "Name") ;

    for(uint8_t i = 0; i < CourseList.size(); i++) {
        ui->listWidget->addItem(CourseList.at(i));
    }

    // Selects the first course by default
    ui->listWidget->item(0)->setSelected(true);

    // Sets current row to 0. This is done due to a weird error that's been
    // happening with CurrentRow return value
    ui->listWidget->setCurrentRow(0);
}



MyCourseDialog::~MyCourseDialog()
{
    delete ui;
}


void MyCourseDialog::on_listWidget_itemSelectionChanged()
{
    ui->listWidget_2->clear();
    CurrentClassList.clear();

    // Class Selection Section
    QDomNodeList courses = root.elementsByTagName("Course") ;

    // Fills QStringList with classes for the first course
    if(ui->listWidget->currentRow() == -1) { // Solves a weird bug where currentRow() returns -1 (why, God?!)
        ui->listWidget->setCurrentRow(0) ;
    }

    // Gets the Code for a given class, e.g., S11, S71, S43, etc
    if(courses.at(ui->listWidget->currentRow()).isElement()) {
        CurrentClassList << getEntry(courses.at(ui->listWidget->currentRow()).toElement(), "Class", "Code");
    }

    // Adds the class code to the ListWidget
    for(uint8_t i = 0; i < CurrentClassList.size(); i++) {
        ui->listWidget_2->addItem(CurrentClassList.at(i));
    }
}

void MyCourseDialog::on_pushButton_2_clicked() // "CONTINUE" PUSH_BUTTON
{
    CurrentCourse = ui->listWidget->currentItem()->text() ; ;
    CurrentClass = ui->listWidget_2->currentItem()->text() ; ;

    dialog_ended = close() ;
}

void MyCourseDialog::on_pushButton_clicked() // "BACK" PUSH_BUTTON
{
    close() ;
}
