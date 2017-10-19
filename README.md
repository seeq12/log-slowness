This project intends to show an unexplained performance issue in a debug log scenario on Windows.
Logback's SizeBasedTriggeringPolicy combined with its RollingFileAppender will check the size of a file
before each log file write operation and rollover the file if necessary.

See comments in `src/ShowLogSlowness.java` for more info.

Invoke the test by executing `java -cp out/production/SlowLogFile ShowLogSlowness`
in the root folder of this project.
