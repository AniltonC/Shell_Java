import java.io.File;

/**
 * Classe responsável por deletar um item, ela recebe a string com o nome do
 * diretorio e deleta ele.
 */

public class Delete {

    /**
     * Construror da classe, recebe o diretorio do arquivo a ser deletado e deleta este arquivo
     * por meio do metodo delete da classe File {@code File<import java.io.File>}
     *
     * @param FILE_NAME diretorio do arquivo/pasta a ser deletada
     */
    public Delete(String FILE_NAME) {
        new File(FILE_NAME).delete();
    }

}
