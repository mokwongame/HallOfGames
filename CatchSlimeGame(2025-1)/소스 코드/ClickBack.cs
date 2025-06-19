using UnityEngine;
using UnityEngine.SceneManagement;

public class ClickBack : MonoBehaviour
{
    public void BackScene()
    {
        SceneManager.LoadScene("StartScene");
    }
}
