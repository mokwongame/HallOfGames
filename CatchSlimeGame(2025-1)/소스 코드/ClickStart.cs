using UnityEngine;
using UnityEngine.SceneManagement;

public class ClickStart : MonoBehaviour
{
    public void GameScene()
    {
        SceneManager.LoadScene("GameScene");
    }
}
