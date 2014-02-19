oid FileFinder::start(const QDir& dir)
{
    QApplication::processEvents();

    QStringList listFiles = 
        dir.entryList(m_ptxtMask->text().split(" "), QDir::Files);
    
    foreach (QString file, listFiles) {
        m_ptxtResult->append(dir.absoluteFilePath(file));
    }

    QStringList listDir = dir.entryList(QDir::Dirs);
    foreach (QString subdir, listDir) {
        if (subdir == "." || subdir == "..") {
            continue;
        }
        start(QDir(dir.absoluteFilePath(subdir)));
    }
}