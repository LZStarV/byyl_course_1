#pragma once
#include <QDialog>
#include <QVector>
#include <QString>

class QLineEdit;
class QTableWidget;
class QCheckBox;
class QPushButton;
class QFileDialog;

class SettingsDialog : public QDialog
{
    Q_OBJECT
   public:
    explicit SettingsDialog(QWidget* parent = nullptr);

   private:
    // navigation
    class QListWidget*    navList;
    class QStackedWidget* stacked;
    // page containers
    class QWidget* pagePaths;
    class QWidget* pageWeightsSkip;
    class QWidget* pageLexerId;
    class QWidget* pageGrammar;
    class QWidget* pageI18nDot;
    class QWidget* pageGraphviz;
    class QWidget* pageSemantic;
    QLineEdit*     edtOutDir;
    QPushButton*   btnBrowseOutDir;
    QTableWidget*  tblTiers;
    QCheckBox*     chkSkipBrace;
    QCheckBox*     chkSkipLine;
    QCheckBox*     chkSkipBlock;
    QCheckBox*     chkSkipHash;
    QCheckBox*     chkSkipSingle;
    QCheckBox*     chkSkipDouble;
    QCheckBox*     chkSkipTemplate;
    QCheckBox*     chkEmitIdLexeme;
    QLineEdit*     edtIdentifierNames;
    QPushButton*   btnAddRow;
    QPushButton*   btnDelRow;
    QPushButton*   btnDefaults;
    QPushButton*   btnSave;
    QPushButton*   btnCancel;
    // extra configs
    QLineEdit* edtSyntaxOutDir;
    QLineEdit* edtGraphsDir;
    QCheckBox* chkTokenMapHeur;
    QLineEdit* edtWhitespaces;
    // token header config
    QLineEdit* edtTokPrefix;
    QLineEdit* edtTokNameFirst;
    QLineEdit* edtTokNameRest;
    QLineEdit* edtTokDigitRanges;
    QLineEdit* edtTokGroupSuffix;
    QCheckBox* chkTokGroupSuffixOptional;
    QLineEdit* edtEpsilon;
    QLineEdit* edtEof;
    QLineEdit* edtAug;
    QLineEdit* edtLr1Policy;
    QLineEdit* edtNontermPat;
    QLineEdit* edtMultiOps;
    QLineEdit* edtSingleOps;
    QLineEdit* edtTblMark;
    QLineEdit* edtTblStateId;
    QLineEdit* edtTblStateSet;
    QLineEdit* edtTblEpsCol;
    QLineEdit* edtDotRankdir;
    QLineEdit* edtDotNodeShape;
    QLineEdit* edtDotEpsLabel;
    QLineEdit* edtGraphvizExe;
    QLineEdit* edtGraphvizDpi;
    QLineEdit* edtGraphvizTimeout;
    QLineEdit* edtCfgSearchPaths;
    QLineEdit* edtSemRootPolicy;
    QLineEdit* edtSemChildOrder;
    void       buildUi();
    void       loadCurrent();
    bool       collectAndApply();
    QString    decideSavePath() const;
};
