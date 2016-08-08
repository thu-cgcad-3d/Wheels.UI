#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include <QApplication>
#include <QtOpenGL>

int main(int argc, char *argv[]) {
  Catch::Session session; // There must be exactly once instance

  // writing to session.configData() here sets defaults
  // this is the preferred way to set them

  int returnCode = session.applyCommandLine(argc, argv);
  if (returnCode != 0) // Indicates a command line error
    return returnCode;

  // writing to session.configData() or session.Config() here
  // overrides command line args
  // only do this if you know you need to

  QApplication app(argc, argv);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(4, 3);
  format.setSamples(16);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  //QSurfaceFormat sf = QSurfaceFormat::defaultFormat();
  //sf.setVersion(4, 0); // whatever version
  //sf.setProfile(QSurfaceFormat::CompatibilityProfile);
  //
  //QSurfaceFormat::setDefaultFormat(sf);
  qDebug("OpenGL version: %d.%d",
         QSurfaceFormat::defaultFormat().majorVersion(),
         QSurfaceFormat::defaultFormat().minorVersion());

  return session.run();
}