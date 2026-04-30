

## Update translate files is needed

```bash
cd path_to_project
# windows
C:/Qt/5.15.2/mingw81_64/bin/lupdate . -no-obsolete -recursive -locations relative -ts ./resources/Translate/FileXplorerInstaller_zh_CN.ts
C:/Qt/5.15.2/mingw81_64/bin/linguist ./resources/Translate/FileXplorerInstaller_zh_CN.ts
C:/Qt/5.15.2/mingw81_64/bin/lrelease ./resources/Translate/FileXplorerInstaller_zh_CN.ts -qm ./resources/Translate/FileXplorerInstaller_zh_CN.qm
# linux
/home/ariel/Qt/5.15.2/gcc_64/bin/lupdate . -no-obsolete -recursive -locations relative -ts ./resources/Translate/FileXplorerInstaller_zh_CN.ts
/home/ariel/Qt/5.15.2/gcc_64/bin/linguist ./resources/Translate/FileXplorerInstaller_zh_CN.ts
/home/ariel/Qt/5.15.2/gcc_64/bin/lrelease ./resources/Translate/FileXplorerInstaller_zh_CN.ts -qm ./resources/Translate/FileXplorerInstaller_zh_CN.qm
```
