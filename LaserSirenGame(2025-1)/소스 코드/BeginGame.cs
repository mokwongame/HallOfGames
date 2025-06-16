using UnityEngine;
using UnityEngine.SceneManagement;

public class BeginGame : MonoBehaviour
{
    private void Start()
    {

        Cursor.visible = true;
    }
    public void startGame()
    {
        SceneManager.LoadScene("MoleScene");
    }

    public void exitGame()
    {
        Application.Quit();
        Debug.Log("game quit");
    }


}

