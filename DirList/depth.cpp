QStringList FileFind::dir2files(const QString& path)
{
   QStringList files;
   dir2fileshelper(path, files, 0);
   return files;
}

void FileFind::dir2fileshelper(const QString& path,
                               QStringList& files,
                               int currentDepth)
{
   static const int MAXIMUM_DEPTH = 40; // For example
   if (currentDepth >= MAXIMUM_DEPTH)
   {
      qWarning("Maximum directory depth limit reached.");
      return;
   }

   QDir directory(path);

   QFileInfoList list = directory.entryInfoList(QDir::NoDotAndDotDot | 
                                                QDir::Dirs |
                                                QDir::Files);

   foreach (const QFileInfo& fileInfo, list)
   {
      if (fileInfo.isDir())
      {
         dir2fileshelper(fileInfo.absoluteFilePath(), files, currentDepth+1);
      }
      else
      {
         files << fileInfo.absoluteFilePath();
      }
   }
}