using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ball_spawn : MonoBehaviour
{
    public GameObject ball;
    GameManager gameManager;
    int colorIndex = 0;
    Renderer rendBall = null;

    public AudioClip clipShot;
    AudioSource audioSource;

    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
        if (gameManager == null)
        {
            Debug.LogError("GameManager not found");
        }
        
        audioSource = GetComponent<AudioSource>();
        if (audioSource == null)
        {
            Debug.Log("AudioSource component not found, adding one.");
            audioSource = gameObject.AddComponent<AudioSource>();
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (gameManager.canMakeBall() && Input.GetKeyDown(KeyCode.Space))
        {
            makeBall();
            gameManager.setMakeBall(false);
        }
        else if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            colorIndex++;
            if (colorIndex >= gameManager.getColorSize()) colorIndex = 0;
            if (rendBall != null) rendBall.material.color = gameManager.getColor(colorIndex);
        }
        else if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            colorIndex--;
            if (colorIndex < 0) colorIndex = gameManager.getColorSize() - 1;
            if (rendBall != null) rendBall.material.color = gameManager.getColor(colorIndex);
        }
        else if (Input.GetKeyDown(KeyCode.Escape))
        {
            SceneManager.LoadScene("endgame");
        }
    }

    void makeBall()
    {
        GameObject newBall = Instantiate(ball, transform.position, transform.rotation);
        rendBall = newBall.GetComponent<Renderer>();
        rendBall.material.color = gameManager.getColor(colorIndex);
        if (audioSource != null && clipShot != null)
        {
            audioSource.PlayOneShot(clipShot, 0.3f);
        }
    }
}