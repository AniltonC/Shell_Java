import java.io.File;
import java.io.IOException;

public class Main {

    private static final String FILE_NAME = "";

    public static void main(String[] args) {
        File file = new File(FILE_NAME);
        file.delete();

        String current = null;
        try {
            current = new File(".").getCanonicalPath();
            System.out.println("Current dir:" + current);
        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
