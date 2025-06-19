using UnityEngine;
using UnityEngine.SceneManagement;

public class ClickIntro : MonoBehaviour
{
    public void IntroScene()
    {
        SceneManager.LoadScene("IntroScene");
    }
}
