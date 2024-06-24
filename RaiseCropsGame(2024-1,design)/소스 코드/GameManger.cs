using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public AudioClip clipSeed;
    public AudioClip clipPlant;
    public AudioSource audioSource;
    public TMP_Text textScore;
    public TMP_Text textBestName;
    public TMP_Text textBestScore;
    int score = 0;
    int bestScore = 0;
    string userName;
    string bestName;

    // Start is called before the first frame update
    void Start()
    {

        userName = PlayerPrefs.GetString("userName");

        bestName = PlayerPrefs.GetString("bestName");
        bestScore = PlayerPrefs.GetInt("bestScore");
        if (bestName.Length <= 0)
        {
            bestName = "none";
            bestScore = 0;
        }

        textBestName.text = bestName;
        textBestScore.text = bestScore.ToString();
        print("userName = " + userName);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Q))
        {

            if (score > bestScore)
            {
                PlayerPrefs.SetString("bestName", userName);
                PlayerPrefs.SetInt("bestScore", score);
                print("best : " + userName + " = " + score);

            }
            SceneManager.LoadScene("EndScene");
        }
    }

    public int getScore() { return score; }
    public void incScore()
    {
        score++;
        showScore();
    }
    public void decScore()
    {
        score--;
        showScore();
    }

    public void showScore()
    {
        textScore.text = score.ToString();
    }

    public void playMissile()
    {
        audioSource.PlayOneShot(clipSeed, 1.0f);
    }

    public void playExplosion()
    {
        audioSource.PlayOneShot(clipPlant, 1.0f);
    }



}
