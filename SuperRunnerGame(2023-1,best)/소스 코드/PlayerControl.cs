using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerControl : MonoBehaviour
{
    public float speed = 10.0f;
    public float jumpForce = 600.0f;
    public float downForce = 1500.0f;
    public AudioSource audioSource;
    public AudioClip clipJump, clipGrounding, clipLaugh, clipDragonEx, clipMeteorEx;
    public GameObject flame, Breathing, MeteorEx, Player;
    Rigidbody2D rb2;
    GameManager gameManager;
    int jumpCount = 0;
    // Start is called before the first frame update
    void Start()
    {
        rb2 = GetComponent<Rigidbody2D>();
        gameManager = FindObjectOfType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        // 병진 운동: 좌우
        float xoff = Input.GetAxis("Horizontal") * speed * Time.deltaTime;
        transform.Translate(xoff, 0.0f, 0.0f);
        // 점프
        if (Input.GetKeyDown(KeyCode.Space) && jumpCount < 2)
        {
            jumpCount++;
            // y방향 속도 초기화
            Vector2 vec2 = rb2.velocity;
            vec2.y = 0.0f;
            rb2.velocity = vec2;
            // 점프
            rb2.AddForce(new Vector2(0.0f, jumpForce));
            playSoundJump();
        }
        else if (Input.GetKeyUp(KeyCode.Space) && rb2.velocity.y > 0.0f)
        {
            Vector2 vec2 = rb2.velocity;
            vec2.y *= 0.5f; // vec2.y = vec2.y*0.5f
            rb2.velocity = vec2;
        }
        else if (Input.GetKeyDown(KeyCode.S))
        {

            // y방향 속도 초기화
            Vector2 vec2 = rb2.velocity;
            vec2.y = 0.0f;
            rb2.velocity = vec2;
            // 점프
            rb2.AddForce(new Vector2(0.0f, -jumpForce));
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Ground")
        {
            ContactPoint2D pt2 = collision.GetContact(0);
            if (pt2.normal.y > 0.9f)
            {
                playSoundGrounding();
                gameManager.incScore();
                jumpCount = 0;

            }
        }
        else if (collision.gameObject.tag == "Monster1")
        {
            ContactPoint2D pt2 = collision.GetContact(0);
            gameManager.decScore();

        }


    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.tag == "Death")
        {
            playSoundLaugh();
            Invoke("gameOver", 3.0f);
        }
        else if (collision.tag == "Meteor")
        {
            Instantiate(MeteorEx, transform.position, transform.rotation);
            playSoundMeteorEx();
            Invoke("gameOver", 1.0f);
        }
        else if (collision.tag == "Breathing")
        {
            Instantiate(flame, transform.position, transform.rotation);
            playSoundDragonEx();
            Invoke("gameOver", 0.5f);
        }
    }

    void gameOver()
    {
        Debug.Log("Game over");
        SceneManager.LoadScene("GameOver");
    }


    void playSoundJump()
    {
        audioSource.volume = 0.3f;
        audioSource.PlayOneShot(clipJump, 1.0f);
    }

    void playSoundGrounding()
    {
        audioSource.volume = 1.0f;
        audioSource.PlayOneShot(clipGrounding, 1.0f);
    }

    void playSoundLaugh()
    {
        audioSource.volume = 0.5f;
        audioSource.PlayOneShot(clipLaugh, 1.0f);
    }

    void playSoundDragonEx()
    {
        audioSource.volume = 0.6f;
        audioSource.PlayOneShot(clipDragonEx, 1.0f);
    }


    void playSoundMeteorEx()
    {
        audioSource.volume = 0.7f;
        audioSource.PlayOneShot(clipMeteorEx, 1.0f);
    }
}
