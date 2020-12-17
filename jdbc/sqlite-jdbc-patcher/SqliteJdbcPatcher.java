import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.MatchResult;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class SqliteJdbcPatcher {
    private static class NativeMethod {
        String method;
        String signature;
        String nativeMethod;

        public NativeMethod(String method, String signature, String nativeMethod) {
            signature = signature.replace("org/sqlite/core/DB/", "org/sqlite/core/DB$");
            this.method = method;
            this.signature = signature;
            this.nativeMethod = nativeMethod;
        }

        @Override
        public String toString() {
            return "NativeMethod{" +
                    "method='" + method + '\'' +
                    ", signature='" + signature + '\'' +
                    ", nativeMethod='" + nativeMethod + '\'' +
                    '}';
        }
    }

    private static final Pattern NATIVE_HEADER_PATTERN = Pattern.compile(
            "(/\\*\n" +
            "\\s*\\*\\s*Class:\\s*([^ \\n]+)\n" +
            "\\s*\\*\\s*Method:\\s*([^ \\n]+)\n" +
            "\\s*\\*\\s*Signature:\\s*([^ \\n]+)\n" +
            "\\s*\\*/\n)" +
            "JNIEXPORT ([^ ]+) JNICALL ([^ (\n]+)"
    );

    private static final Pattern NATIVE_SOURCE_PATTERN = Pattern.compile(
            "JNIEXPORT ([^ ]+) JNICALL ([^ (\n]+)"
    );

    private static final Pattern NATIVE_MODULE_PREDEF_PATTERN = Pattern.compile(
            "/\\*@([^@]+)@\\*/"
    );

    private static final Pattern ARGUMENT_KEY_PATTERN = Pattern.compile(
            "^--([^=]+)(=(.*))?$"
    );

    private String moduleName = "sqlitejdbc";
    private final List<NativeMethod> nativeMethods = new ArrayList<>();

    private String readTextFile(File file) throws IOException {
        StringBuilder stringBuilder = new StringBuilder();
        try (FileInputStream fis = new FileInputStream(file)) {
            BufferedReader reader = new BufferedReader(new InputStreamReader(fis));
            String line;
            while ((line = reader.readLine()) != null) {
                stringBuilder.append(line).append("\n");
            }
        }
        return stringBuilder.toString();
    }

    private static void writeTextFile(File file, String content) throws IOException {
        try (FileOutputStream fos = new FileOutputStream(file)) {
            fos.write(content.getBytes(StandardCharsets.UTF_8));
        }
    }

    private String parseAndPatchNativeDbCHeader(File file) throws IOException {
        String source = readTextFile(file);
        source = replaceMatches(NATIVE_HEADER_PATTERN, source, (MatchResult matchResult) -> {
            StringBuilder refinedBuilder = new StringBuilder();

            nativeMethods.add(new NativeMethod(
                    matchResult.group(3), matchResult.group(4), "_" + matchResult.group(6)
            ));

            refinedBuilder.append(matchResult.group(1));
            refinedBuilder.append(matchResult.group(5));
            refinedBuilder.append(" JNICALL _");
            refinedBuilder.append(matchResult.group(6));
            return refinedBuilder.toString();
        });
        return source;
    }

    private String patchNativeDbCSource(File file) throws IOException {
        String source = readTextFile(file);
        source = replaceMatches(NATIVE_SOURCE_PATTERN, source, (MatchResult matchResult) -> {
            StringBuilder refinedBuilder = new StringBuilder();
            String methodName = matchResult.group(2);
            refinedBuilder.append(matchResult.group(1));
            if ("JNI_OnLoad".equalsIgnoreCase(methodName)) {
                methodName = "_sqlitejdbc_jni_onload";
                refinedBuilder.append(" ");
            } else if ("JNI_OnUnload".equalsIgnoreCase(methodName)) {
                methodName = "_sqlitejdbc_jni_onunload";
                refinedBuilder.append(" ");
            } else {
                refinedBuilder.append(" JNICALL _");
            }
            refinedBuilder.append(methodName);
            return refinedBuilder.toString();
        });
        return source;
    }

    private String patchNativeModuleSource(File file) throws IOException {
        String source = readTextFile(file);
        source = replaceMatches(NATIVE_MODULE_PREDEF_PATTERN, source, (MatchResult matchResult) -> {
            StringBuilder refinedBuilder = new StringBuilder();
            String defname = matchResult.group(1);
            if("SQLITEJDBC_MODULE_NAME".equalsIgnoreCase(defname)) {
                return moduleName;
            } else if("METHOD_TABLE".equalsIgnoreCase(defname)) {
                StringBuilder stringBuilder = new StringBuilder();
                for(NativeMethod item : nativeMethods) {
                    stringBuilder.append("  { \"");
                    stringBuilder.append(item.method);
                    stringBuilder.append("\", \"");
                    stringBuilder.append(item.signature);
                    stringBuilder.append("\", ");
                    stringBuilder.append(item.nativeMethod);
                    stringBuilder.append(" }, \n");
                }
                return stringBuilder.toString();
            }
            return matchResult.group(0);
        });
        return source;
    }

    private File sqliteJdbcSrcDirectory = null;
    private File outputDirectory = null;
    private File jniModuleInputFile = null;
    private File nativeDbHeaderInputFile = null;

    private static void showUsage() {
        System.err.println("Usage: java sqlite-jdbc-patcher.class ARGUMENTS");
        System.err.println("  --module-name=...           : native module name. default \"sqlitejdbc\"");
        System.err.println("  --sqlite-jdbc-src=...       : sqlite jdbc source root");
        System.err.println("  --jni-module-input-file=... : jni_module.in.c file path");
        System.err.println("  --nativedb-h-file=...       : NativeDB.h file path");
        System.err.println("  --out=...                   : output directory (NativeDB.c NativeDB.h jni_module.c)");
    }

    private boolean parseArguments(String[] args) {
        int i;
        int completeFlags = 0;
        for (i = 0; i < args.length; i++) {
            String item = args[i];
            Matcher m = ARGUMENT_KEY_PATTERN.matcher(item);
            if (!m.find()) {
                throw new RuntimeException("Unknown argument pattern: " + item);
            }
            String argKey = m.group(1);
            String argValue = m.group(3);

            if ("help".equalsIgnoreCase(argKey)) {
                return false;
            }else if ("sqlite-jdbc-src".equalsIgnoreCase(argKey)) {
                this.sqliteJdbcSrcDirectory = new File(argValue);
                completeFlags |= 0x1;
            } else if ("out".equalsIgnoreCase(argKey)) {
                this.outputDirectory = new File(argValue);
                completeFlags |= 0x2;
            } else if ("jni-module-input-file".equalsIgnoreCase(argKey)) {
                this.jniModuleInputFile = new File(argValue);
                completeFlags |= 0x4;
            } else if ("nativedb-h-file".equalsIgnoreCase(argKey)) {
                this.nativeDbHeaderInputFile = new File(argValue);
                completeFlags |= 0x8;
            } else if ("module-name".equalsIgnoreCase(argKey)) {
                this.moduleName = argValue;
            }
        }
        return completeFlags == 0x0f;
    }

    public static void main(String[] args) throws IOException {
        SqliteJdbcPatcher inst = new SqliteJdbcPatcher();
        boolean argumentsComplete = inst.parseArguments(args);

        if (!argumentsComplete) {
            showUsage();
            System.exit(1);
        }

        File inputSourceFile = new File(inst.sqliteJdbcSrcDirectory, "src/main/java/org/sqlite/core/NativeDB.c");
        File outputSourceFile = new File(inst.outputDirectory, "NativeDB.c");
        File outputHeaderFile = new File(inst.outputDirectory, "NativeDB.h");
        File outputModuleFile = new File(inst.outputDirectory, "jni_module.c");

        writeTextFile(outputHeaderFile, inst.parseAndPatchNativeDbCHeader(inst.nativeDbHeaderInputFile));
        writeTextFile(outputSourceFile, inst.patchNativeDbCSource(inputSourceFile));
        writeTextFile(outputModuleFile, inst.patchNativeModuleSource(inst.jniModuleInputFile));

        System.exit(0);
    }

    @FunctionalInterface
    public interface ReplaceCallback {
        String foundMatch(MatchResult matchResult);
    }

    public String replaceMatches(Pattern pattern, String string, ReplaceCallback callback) {
        final Matcher matcher = pattern.matcher(string);
        while(matcher.find())
        {
            final MatchResult matchResult = matcher.toMatchResult();
            final String replacement = callback.foundMatch(matchResult);
            string = string.substring(0, matchResult.start()) +
                    replacement + string.substring(matchResult.end());
            matcher.reset(string);
        }
        return string;
    }
}
