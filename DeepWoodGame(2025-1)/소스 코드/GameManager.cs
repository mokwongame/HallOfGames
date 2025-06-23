using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    public int score = 100;
    public static GameManager Instance;

    [SerializeField] TMP_Text textScore;
    [SerializeField] Slider hpSlider;

    [SerializeField] AudioSource audioSource;
    [SerializeField] List<AudioClip> clips = new List<AudioClip>();

    PlayerController playerController;
    void Start()
    {
        Instance = this;
        Application.targetFrameRate = 60;
        playerController = FindAnyObjectByType<PlayerController>();
        updateScore();
        updateHp();
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            EndScene();
        }
    }

    public void incScore(int s)
    {
        score += s;
        updateScore();
    }

    public void updateScore()
    {
        textScore.text = score.ToString();
    }

    public void updateHp()
    {
        hpSlider.value = playerController.Hp / (float)playerController.MaxHp;
    }

    public void EndScene()
    {
        PlayerPrefs.SetInt("Score", GameManager.Instance.score);
        SceneManager.LoadScene("EndScene");
    }
    public void PlaySound(string name)
    {
        switch (name)
        {
            case "EnemyHit":
                audioSource.PlayOneShot(clips[0], 0.8f);
                break;
            case "PlayerHit":
                audioSource.PlayOneShot(clips[1], 0.8f);
                break;
            case "EnemyAttack":
                audioSource.PlayOneShot(clips[2], 0.5f);
                break;
            case "PlayerAttack":
                audioSource.PlayOneShot(clips[3], 0.8f);
                break;
            case "Bullet":
                audioSource.PlayOneShot(clips[4], 0.3f);
                break;
        }

    }
}
