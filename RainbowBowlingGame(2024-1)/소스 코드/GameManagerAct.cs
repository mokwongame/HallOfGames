using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement; // LoadScene을 위해 필요
using TMPro;

public class GameManager : MonoBehaviour
{
    public TMP_Text textScore;
    public AudioClip clipShot;
    public AudioClip gamestartvoice;
    bool bMakeBall = true;
    int score = 0;
    AudioSource audioSource;

    // Start is called before the first frame update
    void Start()
    {
        audioSource = GetComponent<AudioSource>();
        if (audioSource == null)
        {
            Debug.LogError("AudioSource component not found");
        }
        showScore(); // 시작할 때 점수 표시
    }

    // Update is called once per frame
    void Update()
    {
        // score가 100 이상이면 endgame 씬으로 전환
        if (score >= 100)
        {
            SceneManager.LoadScene("endgame");
        }
    }

    public bool canMakeBall()
    {
        return bMakeBall;
    }

    public void setMakeBall(bool bMake)
    {
        bMakeBall = bMake;
    }

    public int getScore() { return score; }

    public void setScore(int newScore)
    {
        score = newScore;
        showScore(); // 점수가 변경될 때마다 표시 업데이트
    }

    public void inScore()
    {
        score++;
        showScore();
    }

    public void incBestScore()
    {
        score += 10;
        showScore();
    }

    public int getColorSize() { return 8; }

    public Color getColor(int colorIndex)
    {
        switch (colorIndex)
        {
            case 0: return Color.red;
            case 1: return Color.green;
            case 2: return Color.blue;
            case 3: return Color.magenta;
            case 4: return Color.yellow;
            case 5: return Color.cyan;
            case 6: return Color.white;
            case 7: return Color.black;
            default: return Color.black;
        }
    }

    public Color getRandomColor()
    {
        int colorIndex = Random.Range(0, getColorSize());
        return getColor(colorIndex);
    }

    public void showScore()
    {
        textScore.text = score.ToString();
    }

    public void playBallshot()
    {
        audioSource.PlayOneShot(clipShot, 1.0f); // clipShot: MP3 파일이 연결될 AudioClip; 1.0f: 소리를 100%로 연주
    }

    public void playStartGameVoice()
    {
        audioSource.PlayOneShot(gamestartvoice, 0.5f);
    }
}