This project intends to show an unexplained performance issue in a debug log scenario **on Windows**.

Logback's SizeBasedTriggeringPolicy combined with its RollingFileAppender will check the size of a file
before each log file write operation and rollover the file if necessary.

Java Version
============

See comments in `src/ShowLogSlowness.java` for more info.

Invoke the test by executing `java -cp out/production/SlowLogFile ShowLogSlowness`
in the root folder of this project.

Requires Java 8 or later.

C Version
=========

The C Version of the repro takes Java out of the picture completely, but the problem is the same.

Invoke the test by executing `SlowLogFile.exe` in the root folder of this project.
