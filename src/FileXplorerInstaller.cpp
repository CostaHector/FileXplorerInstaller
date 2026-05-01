#include "FileXplorerInstaller.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QDesktopServices>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

constexpr const char* ThirdPartyDependencyConfig::LIBNAME_CSTR[];
ThirdPartyDependencyConfig::ThirdPartyDependencyConfig(int pageId, const LIBNAME libName, QWidget* parent)
  : QWizardPage{parent}
  , mLibName{libName} {
  const char* pHint{LIBNAME_CSTR[(int) mLibName]};
  const QString libDisplayName{pHint};

  setTitle(QString::number(pageId) + tr(". Install ") + libDisplayName + tr(" Dependency"));

  setCommitPage(true);

  mLog = new QPlainTextEdit{this};
  mLog->setReadOnly(true);

  mDownloadPageUrl = QUrl{"https://www.baidu.com"};

  switch (mLibName) {
    case LIBNAME::FFMPEG:
      mDefaultBinaryPath = QString{R"(C:\home\ffmpeg\bin)"};
      mLog->setPlainText(tr("1. Download FFmpeg (ffmpeg-7.1.1-full_build-shared.7z, 44.4 MB) from the official website\n"
                            "2. Extract the archive to: C:\\home\\ffmpeg\n"
                            "3. Add the 'bin' folder to your user PATH environment variable"));
      break;
    case LIBNAME::MEDIA_INFO:
      mDefaultBinaryPath = QString{R"(C:\home\mediaInfo\bin)"};
      mLog->setPlainText(tr("1. Download MediaInfo (mediaInfo.7z, 2.2 MB) from the official website\n"
                            "2. Extract the archive to: C:\\home\\mediaInfo\n"
                            "3. Add the 'bin' folder to your user PATH environment variable"));
      break;
    case LIBNAME::OPENSSL:
      mDefaultBinaryPath = QString{R"(C:\Program Files\OpenSSL-Win64\bin)"};
      mLog->setPlainText(tr("1. Download OpenSSL (Win64OpenSSL-3_5_1.msi, 280 MB) from the official website\n"
                            "2. Install to: C:\\Program Files\\OpenSSL-Win64\n"
                            "3. During installation, select 'Copy OpenSSL DLLs to: The OpenSSL binaries (/bin) directory'\n"
                            "4. Add the 'bin' folder to your user PATH environment variable"));
      break;
    default:
      return;
  }

  mDownloadDependencyLibBtn = new QPushButton{tr("Open %1 Download Page").arg(libDisplayName), this};
  mDownloadDependencyLibBtn->setToolTip(tr("Open browser to download %1").arg(libDisplayName));

  mDownloadedToFolder = new QLineEdit{this};
  mDownloadedToFolder->setText(mDefaultBinaryPath);
  mDownloadedToFolder->setPlaceholderText(tr("Enter the installation path for %1").arg(libDisplayName));

  mAddToSystemVariableBtn = new QPushButton{tr("Add to user PATH"), this};
  mAddToSystemVariableBtn->setToolTip(tr("Add the selected folder to the user PATH environment variable"));

  QFormLayout* lo = new QFormLayout{this};
  lo->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
  lo->setLabelAlignment(Qt::AlignRight);

  lo->addRow(tr("Installation Steps:"), mLog);
  lo->addRow(tr("Download:"), mDownloadDependencyLibBtn);
  lo->addRow(tr("Binary Path:"), mDownloadedToFolder);
  lo->addRow("", mAddToSystemVariableBtn);

  setLayout(lo);

  connect(mDownloadDependencyLibBtn, &QPushButton::clicked, this, [this]() { QDesktopServices::openUrl(mDownloadPageUrl); });
  connect(mAddToSystemVariableBtn, &QPushButton::clicked, this, &ThirdPartyDependencyConfig::onAddThirdPartyBinPathToSystemVariablePath);
}

void ThirdPartyDependencyConfig::onAddThirdPartyBinPathToSystemVariablePath() {
  const QString binPath = mDownloadedToFolder->text();
  if (!QFileInfo{binPath}.isDir()) {
    mLog->appendPlainText(QString{"bin path[%1] invalid"}.arg(binPath));
    return;
  }
  const QString binPathSystem = QDir::toNativeSeparators(binPath);
  const QString hintTitle{tr("Add Path to User PATH")};
  const QString hintMessage{tr("Click OK to copy the path to clipboard and open Environment Variables:\n\n"
                               "%1\n\n"
                               "Instructions:\n"
                               "1. In the Environment Variables window, select 'Path' in the 'User variables' section\n"
                               "2. Click 'Edit...'\n"
                               "3. Click 'New' and paste the path\n"
                               "4. Click 'OK' to save\n\n"
                               "Note: This will only affect your user account, not all users.")
                                .arg(binPathSystem)};
  if (QMessageBox::information(this, hintTitle, hintMessage, QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No) != QMessageBox::StandardButton::Yes) {
    return;
  }
  QApplication::clipboard()->setText(binPathSystem);
  QProcess::startDetached("rundll32", QStringList{"sysdm.cpl,EditEnvironmentVariables"});
}

QWizardPage* FileXplorerInstaller::CreateIntroductionPage(QWidget* parent) {
  auto* page = new QWizardPage{parent};
  page->setTitle(tr("Welcome to FileXplorer Setup"));
  page->setSubTitle(tr("This wizard will guide you through the installation process."));
  page->setButtonText(QWizard::WizardButton::NextButton, tr("Next"));
  page->setButtonText(QWizard::WizardButton::CancelButton, tr("Cancel"));
  page->setFinalPage(false);
  return page;
}

QWizardPage* FileXplorerInstaller::CreateFinishedPage(QWidget* parent) {
  auto* page = new QWizardPage{parent};
  page->setTitle(tr("Completing FileXplorer Setup"));
  page->setSubTitle(tr("FileXplorer has been successfully installed on your computer."));
  page->setButtonText(QWizard::WizardButton::FinishButton, tr("Finish"));
  page->setFinalPage(true);
  return page;
}

enum PAGE_NAME { INTRODUCTION_PAGE = 0, FFMPEG_PAGE, MEDIA_INFO_PAGE, OPENSSL_PAGE, FINISHED_PAGE };

FileXplorerInstaller::FileXplorerInstaller(QWidget* parent)
  : QWizard{parent} {
  setWindowTitle(tr("FileXplorer Installation Guide"));

  addPage(CreateIntroductionPage(this));
  addPage(new ThirdPartyDependencyConfig{FFMPEG_PAGE, ThirdPartyDependencyConfig::LIBNAME::FFMPEG, this});
  addPage(new ThirdPartyDependencyConfig{MEDIA_INFO_PAGE, ThirdPartyDependencyConfig::LIBNAME::MEDIA_INFO, this});
  addPage(new ThirdPartyDependencyConfig{OPENSSL_PAGE, ThirdPartyDependencyConfig::LIBNAME::OPENSSL, this});
  addPage(CreateFinishedPage(parent));

  setStartId(INTRODUCTION_PAGE);
}
