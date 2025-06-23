using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneBGMPlayer : MonoBehaviour
{
    public AudioClip bgmStart;
    public AudioClip bgmGame;
    public AudioClip bgmEnd;

    private AudioSource audioSource;

    void Awake()
    {
        audioSource = gameObject.AddComponent<AudioSource>();
        audioSource.loop = true;
        PlayBGMForCurrentScene();
    }

    void PlayBGMForCurrentScene()
    {
        string scene = SceneManager.GetActiveScene().name;
        AudioClip selectedClip = null;

        if (scene == "BeginScene") selectedClip = bgmStart;
        else if (scene == "DuckScene" || scene == "GameScene") selectedClip = bgmGame;
        else if (scene == "EndScene") selectedClip = bgmEnd;

        if (selectedClip != null)
        {
            audioSource.clip = selectedClip;
            audioSource.Play();
        }
    }
}


