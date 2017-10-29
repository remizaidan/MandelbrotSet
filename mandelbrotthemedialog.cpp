#include "mandelbrotthemedialog.h"
#include "ui_mandelbrotthemedialog.h"
#include "mandelbrotthememanager.h"

#include <QPixmap>
#include <QPainter>
#include <QColorDialog>
#include <QLineEdit>
#include <QDebug>

MandelbrotThemeDialog::MandelbrotThemeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MandelbrotThemeDialog)
{
    ui->setupUi(this);

    QSizePolicy spRetain = ui->startColorButton->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    ui->startColorButton->setSizePolicy(spRetain);

    spRetain = ui->endColorButton->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    ui->endColorButton->setSizePolicy(spRetain);

    spRetain = ui->seedSpinBox->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    ui->seedSpinBox->setSizePolicy(spRetain);

    spRetain = ui->seedButton->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    ui->seedButton->setSizePolicy(spRetain);

    connect(ui->nameCombo->lineEdit(), SIGNAL(textEdited(QString)),
            this, SLOT(onNameComboTextEdited(QString)));
}

MandelbrotThemeDialog::~MandelbrotThemeDialog()
{
    delete ui;
}

void MandelbrotThemeDialog::setThemeManager(MandelbrotThemeManager *themeManager)
{
    m_themeManager.copyFrom(themeManager);

    ui->nameCombo->blockSignals(true);
    ui->nameCombo->addItems(m_themeManager.getThemes());
    ui->nameCombo->setCurrentText(m_themeManager.getCurrentThemeName());
    ui->nameCombo->blockSignals(false);

    loadThemeInfo();
}

const MandelbrotThemeManager &MandelbrotThemeDialog::getThemeManager()
{
    return m_themeManager;
}

QPixmap MandelbrotThemeDialog::getColorButtonIcon(const QColor &color)
{
    QPixmap pixmap(22, 22);
    pixmap.fill(color);

    QPainter painter(&pixmap);
    painter.setPen(QColor(128, 128, 128));
    painter.drawLine(0,0,0,21);
    painter.drawLine(0,0,0,10);
    painter.drawLine(0,0,21,0);
    painter.drawLine(0,0,20,0);
    painter.setPen(QColor(236, 236, 236));
    painter.drawLine(21,21,0,21);
    painter.drawLine(21,21,0,20);
    painter.drawLine(21,21,21,0);
    painter.drawLine(21,21,20,0);

    return pixmap;
}

QPixmap MandelbrotThemeDialog::getPalettePreviewPixmap()
{
    const MandelbrotTheme &theme = m_themeManager.getCurrentTheme();
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentThemeInfo();

    QPixmap pixmap(themeInfo.getNColors(), 16);
    pixmap.fill(QColor(0,0,0));
    QPainter painter(&pixmap);
    for(int i=0; i<themeInfo.getNColors(); i++) {
        painter.setPen(theme.getPaletteColor(i));
        painter.drawLine(i, 0, i, 15);
    }

    return pixmap;
}

bool MandelbrotThemeDialog::getColor(QColor &color)
{

    QColorDialog dlg(this);
    dlg.setOption(QColorDialog::DontUseNativeDialog, true);
    dlg.setCurrentColor(color);
    dlg.exec();

    if(dlg.result() == QDialog::Accepted) {

        color = dlg.selectedColor();
        return true;
    }
    return false;
}

void MandelbrotThemeDialog::showHideWidgets()
{

    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentThemeInfo();

    switch(themeInfo.getThemeType()) {
    case MandelbrotThemeInfo::BiColor:
        ui->convergedColorButton->show();
        ui->convergedLabel->show();
        ui->divergedColorButton->show();
        ui->divergedLabel->show();
        ui->paletteTypeLabel->hide();
        ui->paletteTypeCombo->hide();
        ui->nColorsLabel->hide();
        ui->nColorsSpinBox->hide();
        ui->palettePreviewLabel->hide();
        ui->palettePreview->hide();
        ui->startColorButton->hide();
        ui->endColorButton->hide();
        ui->seedLabel->hide();
        ui->seedSpinBox->hide();
        ui->seedButton->hide();
        break;
    default:
        ui->convergedColorButton->show();
        ui->convergedLabel->show();
        ui->divergedColorButton->hide();
        ui->divergedLabel->hide();
        ui->paletteTypeLabel->show();
        ui->paletteTypeCombo->show();
        ui->nColorsLabel->show();
        ui->nColorsSpinBox->show();
        ui->palettePreviewLabel->show();
        ui->palettePreview->show();
        switch(themeInfo.getPaletteType()) {
        case MandelbrotThemeInfo::Rainbow:
            ui->startColorButton->hide();
            ui->endColorButton->hide();
            ui->seedLabel->hide();
            ui->seedSpinBox->hide();
            ui->seedButton->hide();
            break;
        case MandelbrotThemeInfo::Randomized:
            ui->startColorButton->hide();
            ui->endColorButton->hide();
            ui->seedLabel->show();
            ui->seedSpinBox->show();
            ui->seedButton->show();
            break;
        default:
            ui->startColorButton->show();
            ui->endColorButton->show();
            ui->seedLabel->hide();
            ui->seedSpinBox->hide();
            ui->seedButton->hide();
            break;
        }
        break;
    }
}

void MandelbrotThemeDialog::loadThemeInfo()
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentThemeInfo();

    ui->typeCombo->blockSignals(true);
    ui->typeCombo->clear();
    ui->typeCombo->addItems(themeInfo.getThemeTypes());
    ui->typeCombo->setCurrentText(themeInfo.getThemeTypeName(themeInfo.getThemeType()));
    ui->typeCombo->blockSignals(false);
    ui->convergedColorButton->setIcon(getColorButtonIcon(themeInfo.getConvergedColor()));
    ui->divergedColorButton->setIcon(getColorButtonIcon(themeInfo.getDivergedColor()));

    ui->nColorsSpinBox->setValue(themeInfo.getNColors());

    ui->paletteTypeCombo->blockSignals(true);
    ui->paletteTypeCombo->clear();
    ui->paletteTypeCombo->addItems(themeInfo.getPaletteTypes());
    ui->paletteTypeCombo->setCurrentText(themeInfo.getPaletteTypeName(
                                             themeInfo.getPaletteType()));
    ui->paletteTypeCombo->blockSignals(false);

    ui->startColorButton->setIcon(getColorButtonIcon(themeInfo.getStartColor()));
    ui->endColorButton->setIcon(getColorButtonIcon(themeInfo.getEndColor()));
    ui->palettePreview->setScaledContents(true);
    ui->palettePreview->setPixmap(getPalettePreviewPixmap());

    ui->seedSpinBox->blockSignals(true);
    ui->seedSpinBox->setValue(themeInfo.getRandomSeed());
    ui->seedSpinBox->blockSignals(false);

    showHideWidgets();
}


void MandelbrotThemeDialog::on_nameCombo_currentIndexChanged(const QString &arg1)
{
    m_themeManager.setTheme(arg1);
    loadThemeInfo();
}

void MandelbrotThemeDialog::onNameComboTextEdited(const QString &arg1)
{
    m_themeManager.renameCurrentTheme(arg1);
}

void MandelbrotThemeDialog::on_typeCombo_currentTextChanged(const QString &arg1)
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentTheme().getThemeInfo();
    themeInfo.setThemeType(MandelbrotThemeInfo::getThemeType(arg1));
    m_themeManager.getCurrentTheme().setThemeInfo(themeInfo);
    showHideWidgets();
}

void MandelbrotThemeDialog::on_convergedColorButton_clicked()
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentTheme().getThemeInfo();

    QColor c = themeInfo.getConvergedColor();
    bool accepted = getColor(c);
    if(accepted) {
        themeInfo.setConvergedColor(c);
        m_themeManager.getCurrentTheme().setThemeInfo(themeInfo);
        ui->convergedColorButton->setIcon(getColorButtonIcon(c));
    }
}

void MandelbrotThemeDialog::on_divergedColorButton_clicked()
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentTheme().getThemeInfo();

    QColor c = themeInfo.getDivergedColor();
    bool accepted = getColor(c);
    if(accepted) {
        themeInfo.setDivergedColor(c);
        m_themeManager.getCurrentTheme().setThemeInfo(themeInfo);
        ui->divergedColorButton->setIcon(getColorButtonIcon(c));
    }
}

void MandelbrotThemeDialog::on_startColorButton_clicked()
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentTheme().getThemeInfo();

    QColor c = themeInfo.getStartColor();
    bool accepted = getColor(c);
    if(accepted) {
        themeInfo.setStartColor(c);
        m_themeManager.getCurrentTheme().setThemeInfo(themeInfo);
        ui->startColorButton->setIcon(getColorButtonIcon(c));
        ui->palettePreview->setPixmap(getPalettePreviewPixmap());
    }
}

void MandelbrotThemeDialog::on_endColorButton_clicked()
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentTheme().getThemeInfo();

    QColor c = themeInfo.getEndColor();
    bool accepted = getColor(c);
    if(accepted) {
        themeInfo.setEndColor(c);
        m_themeManager.getCurrentTheme().setThemeInfo(themeInfo);
        ui->endColorButton->setIcon(getColorButtonIcon(c));
        ui->palettePreview->setPixmap(getPalettePreviewPixmap());
    }
}

void MandelbrotThemeDialog::on_nColorsSpinBox_valueChanged(int arg1)
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentTheme().getThemeInfo();

    themeInfo.setNColors(arg1);
    m_themeManager.getCurrentTheme().setThemeInfo(themeInfo);
    ui->palettePreview->setPixmap(getPalettePreviewPixmap());
}

void MandelbrotThemeDialog::on_paletteTypeCombo_currentTextChanged(const QString &arg1)
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentTheme().getThemeInfo();

    themeInfo.setPaletteType(MandelbrotThemeInfo::getPaletteType(arg1));
    m_themeManager.getCurrentTheme().setThemeInfo(themeInfo);
    ui->palettePreview->setPixmap(getPalettePreviewPixmap());
    showHideWidgets();
}

void MandelbrotThemeDialog::on_newThemeButton_clicked()
{

    QString newThemeName = "New Theme";
    int i = 1;
    while(ui->nameCombo->findText(newThemeName) >= 0) {
        i++;
        newThemeName = "New Theme "+QString::number(i);
    }

    MandelbrotThemeInfo themeInfo;
    themeInfo.setName(newThemeName);
    m_themeManager.addTheme(themeInfo);
    m_themeManager.setTheme(newThemeName);

    ui->nameCombo->addItem(newThemeName);
    ui->nameCombo->setCurrentText(newThemeName);
    ui->nameCombo->setFocus();
    ui->nameCombo->lineEdit()->selectAll();
}


void MandelbrotThemeDialog::on_seedSpinBox_valueChanged(int arg1)
{
    MandelbrotThemeInfo themeInfo = m_themeManager.getCurrentTheme().getThemeInfo();

    themeInfo.setRandomSeed(arg1);
    m_themeManager.getCurrentTheme().setThemeInfo(themeInfo);
    ui->palettePreview->setPixmap(getPalettePreviewPixmap());
}


void MandelbrotThemeDialog::on_seedButton_clicked()
{
    int seed = rand() % ui->seedSpinBox->maximum();
    ui->seedSpinBox->setValue(seed);
}
