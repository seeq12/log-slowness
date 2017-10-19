import java.io.File;
import java.io.FileOutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class ShowLogSlowness {
    public static void main(String[] argv) {

        // This function intends to show an unexplained performance issue in a debug log scenario on Windows.
        // Logback's SizeBasedTriggeringPolicy combined with its RollingFileAppender will check the size of a file
        // before each log file write operation and rollover the file if necessary.
        //
        // It appears that the combination of having a FileOutputStream open, checking the file length, then appending
        // to the file can be very expensive depending on the existing content of the file. (The File.length() call
        // appears to be the expensive call, but it's the combination of all the aforementioned conditions that are
        // important.)
        //
        // The utility appends 20 lines to two different "log files". The only difference between them is that all
        // dollar-sign ($) characters in the first ~200 lines have been removed from the "fast" file.
        //
        // Testing this on Windows 10 on a beefy development machine shows that the 20 append operations take ~100ms
        // in the fast case and ~2500ms in the slow case.
        //
        // This problem can slow an application to a crawl!

        Path currentDirectory = Paths.get(System.getProperty("user.dir"));
        try {
            Files.list(currentDirectory).filter(p1 -> p1.toString().endsWith(".log")).forEach(p1 -> {
                try {
                    Path p2 = Paths.get(p1.toString() + ".test");
                    Files.copy(p1, p2);
                    File file = p2.toFile();
                    try (FileOutputStream outputStream = new FileOutputStream(file, true)) {
                        long start = System.currentTimeMillis();
                        long length = 0;
                        for (int i = 0; i < 20; i++) {
                            length = file.length();
                            outputStream.write("Appended line\n".getBytes());
                        }
                        long stop = System.currentTimeMillis();
                        System.out.format("%s -> %6d ms -> %d bytes\n", file, stop - start, length);
                    }
                    Files.delete(p2);
                } catch (Exception e) {
                    System.out.format("Exception: %s", e);
                }
            });
        } catch (Exception e) {
            System.out.format("Exception: %s", e);
        }
    }
}
