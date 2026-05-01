#ifndef FILEXPLORERINSTALLER_H
#define FILEXPLORERINSTALLER_H

#include <QWizard>
#include <QWizardPage>
#include <QUrl>

class QPlainTextEdit;
class QLineEdit;

class ThirdPartyDependencyConfig : public QWizardPage {
  Q_OBJECT
public:
  enum class LIBNAME {
    FFMPEG = 0,
    MEDIA_INFO,
    OPENSSL,
    INVALID_END,
  };
  static constexpr const char* LIBNAME_CSTR[(int) LIBNAME::INVALID_END]{"ffmpeg", "mediaInfo", "openssl"};

  explicit ThirdPartyDependencyConfig(int pageId, const LIBNAME libName, QWidget* parent = nullptr);

private:
  void onAddThirdPartyBinPathToSystemVariablePath();

  LIBNAME mLibName{LIBNAME::INVALID_END};
  QUrl mDownloadPageUrl;
  QString mDefaultBinaryPath;
  QPlainTextEdit* mLog{nullptr};
  QLineEdit* mDownloadedToFolder{nullptr};
  QPushButton* mDownloadDependencyLibBtn{nullptr};
  QPushButton* mAddToSystemVariableBtn{nullptr};
};

class FileXplorerInstaller : public QWizard {
  Q_OBJECT
public:
  explicit FileXplorerInstaller(QWidget* parent = nullptr);
private:
  QWizardPage* CreateIntroductionPage(QWidget* parent);
  QWizardPage* CreateFinishedPage(QWidget* parent);
};
#endif // FILEXPLORERINSTALLER_H
