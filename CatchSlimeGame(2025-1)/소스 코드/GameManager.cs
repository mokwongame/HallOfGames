using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public float axisSpeed = 10.0f;
    public int score = 0;
    public TMP_Text textScore;
    public AudioClip hitSound;
    public AudioClip failSound;
    public AudioSource audioSource;
    
    void Start()
    {
        
    }

    
    void Update()
    {
        if (score <= -5)
        {
            SceneManager.LoadScene("End");
        }
        else if (score >= 10)
        {
            SceneManager.LoadScene("Win");
        }
    }
    public void incScore()
    {
        score += 1;
        textScore.text = $"Score: {score}";
    }
    public void decScore()
    {
        score -= 1;
        textScore.text = $"Score: {score}";
    }
    public void incScore2()
    {
        score += 3;
        textScore.text = $"Score: {score}";
    }
    public void decScore2()
    {
        score -= 2;
        textScore.text = $"Score: {score}";
    }

    public void playHit()
    {
        float prevVol = audioSource.volume;
        audioSource.volume = 1.0f;
        audioSource.PlayOneShot(hitSound);
        audioSource.volume = prevVol;
    }
    public void playFail()
    {
        float prevVol = audioSource.volume;
        audioSource.volume = 1.0f;
        audioSource.PlayOneShot(failSound);
        audioSource.volume = prevVol;
    }
}
