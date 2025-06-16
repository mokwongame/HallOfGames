using NUnit.Framework;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;
using UnityEngine.SceneManagement;
using UnityEngine.SocialPlatforms.Impl;
using UnityEngine.Tilemaps;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{

    int score = 0;
    public static GameManager Instance;

    [SerializeField] TMP_Text textScore;

    [SerializeField] AudioSource audioSource;
    [SerializeField] List<AudioClip> clips = new List<AudioClip>();

    [SerializeField] Slider timer;
    float currentTime;
    [SerializeField] float timeMax;


    bool endGame = false;
    [SerializeField] Volume volume;
    Vignette vignette;
    void Start()
    {
        Instance = this;
        updateScore();
        currentTime = timeMax;
        volume.profile.TryGet(out vignette);
        Cursor.visible = false;

    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            SceneManager.LoadScene("EndScene");
            PlayerPrefs.SetInt("Score", score);
            if (score > PlayerPrefs.GetInt("HighScore"))
            {
                PlayerPrefs.SetInt("HighScore", score);
            }
        }
        if (Input.GetKeyDown(KeyCode.R))
        {
            PlayerPrefs.SetInt("HighScore", 0);
        }

        currentTime -= Time.deltaTime;
        UpdateTimer();
        if (currentTime <= 0)
        {
            endGame = true;
            StartCoroutine("EndGame");
        }

    }

    public int getScore()
    {
        return score;
    }
    public void incScore(int num)
    {
        score += num;
        score = score <= 0 ? 0 : score;
        updateScore();
    }
    public void desScore(int num)
    {
        score-=num;
        updateScore();
    }
    void updateScore()
    {
        textScore.text = score.ToString();
    }

    public void incTime(int num)
    {
        currentTime += num;
        currentTime = timeMax < currentTime ? timeMax : currentTime;
    }
    public void desTime()
    {
        currentTime--;
    }
    void UpdateTimer()
    {
        timer.value = currentTime / timeMax;
    }

    public void PlaySound(string name)
    {
        switch (name)
        {
            case "FireBullet":
                audioSource.PlayOneShot(clips[0], 0.3f);
                break;
            case "Bomb":
                audioSource.PlayOneShot(clips[1], 0.6f);
                break;
            case "Shield":
                audioSource.PlayOneShot(clips[2], 0.2f);
                break;

        }

    }

    IEnumerator EndGame()
    {
        while (vignette.smoothness.value < 0.9)
        {
            vignette.intensity.Override(vignette.intensity.value + Time.deltaTime);
            vignette.smoothness.Override(vignette.smoothness.value + Time.deltaTime);
            yield return null;
        }
        vignette.center.value = new Vector2(0.5f, 2);
        yield return null;
        SceneManager.LoadScene("EndScene"); 
        PlayerPrefs.SetInt("Score", score);
        if (score > PlayerPrefs.GetInt("HighScore"))
        {
            PlayerPrefs.SetInt("HighScore", score);
        }


    }
}
