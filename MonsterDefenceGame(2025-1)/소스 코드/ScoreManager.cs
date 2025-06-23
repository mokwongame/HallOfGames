using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class ScoreManager : MonoBehaviour
{
    public static ScoreManager Instance { get; private set; }

    public int score = 0;
    public TMP_Text scoreText;

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(gameObject);

            SceneManager.sceneLoaded += OnSceneLoaded;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    private void Start()
    {
        UpdateScoreText();
    }

    private void OnSceneLoaded(Scene scene, LoadSceneMode mode)
    {
        // 새 씬에서 scoreText 다시 찾기
        scoreText = GameObject.Find("ScoreText")?.GetComponent<TMP_Text>();
        UpdateScoreText();
    }

    public void AddScore(int amount)
    {
        score += amount;
        Debug.Log($"Score added: {amount}, total score: {score}");
        UpdateScoreText();
        CheckStageProgress();
    }


    void UpdateScoreText()
    {
        if (scoreText != null)
        {
            scoreText.text = "Score: " + score.ToString();
        }
    }

    void CheckStageProgress()
    {
        string currentScene = SceneManager.GetActiveScene().name;

        if (currentScene == "Stage1" && score >= 30)
        {
            SceneManager.LoadScene("Stage2");
        }
        else if (currentScene == "Stage2" && score >= 60)
        {
            SceneManager.LoadScene("Stage3");
        }
        else if (currentScene == "Stage3" && score >= 100)
        {
            SceneManager.LoadScene("ClearScene");
        }
    }

    private void OnDestroy()
    {
        SceneManager.sceneLoaded -= OnSceneLoaded;
    }
}
