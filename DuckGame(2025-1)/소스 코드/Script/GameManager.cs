using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using System.Collections;

public class GameManager : MonoBehaviour
{
    public float speedBack = 2.0f;
    public float speedPlayer = 4.0f;
    public float speedGoblin = 2.0f;
    public float forcePlayer = 3000.0f;
    public float forceSpear = 1000.0f;

    public int scorePerApple = 1;
    public int scorePerStone = 1;
    public TMP_Text textScoreApple;
    public TMP_Text textScoreStone;

    // Bang 연출용
    public Image bangImage;
    public Sprite baseSprite;
    public Sprite appleSprite;
    public Sprite stoneSprite;

    Coroutine currentBangRoutine;

    int scoreApple = 0;
    int scoreStone = 0;

    void Start()
    {
        updateScore();
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            SceneManager.LoadScene("EndScene");
        }
    }

    public void AddAppleScore()
    {
        scoreApple += scorePerApple;
        updateScore();
        checkEndGame();
    }

    public void AddStoneScore()
    {
        scoreStone += scorePerStone;
        updateScore();
        checkEndGame();
    }

    void checkEndGame()
    {
        if (scoreApple + scoreStone >= 10)
        {
            Debug.Log("Game Over: Score reached 10");

            GameResult.endReason = EndReason.ScoreReached;  // ✅ 여기 추가
            SceneManager.LoadScene("EndScene");
        }
    }


    void updateScore()
    {
        textScoreApple.text = $"Apple: {scoreApple}";
        textScoreStone.text = $"Stone: {scoreStone}";
    }

    public void ShowAppleBang()
    {
        ShowBang(appleSprite);
    }

    public void ShowStoneBang()
    {
        ShowBang(stoneSprite);
    }

    void ShowBang(Sprite sprite)
    {
        if (currentBangRoutine != null)
            StopCoroutine(currentBangRoutine);

        currentBangRoutine = StartCoroutine(BangRoutine(sprite));
    }

    IEnumerator BangRoutine(Sprite sprite)
    {
        bangImage.sprite = sprite;
        yield return new WaitForSeconds(2.0f);
        bangImage.sprite = baseSprite;
        currentBangRoutine = null;
    }

}
