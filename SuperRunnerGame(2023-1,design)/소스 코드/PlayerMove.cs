using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class PlayerMove : MonoBehaviour
{
    public float speed = 5.0f;
    public float jumpForce = 300.0f;
    int jumpCount = 0;
    float GameOverTime = 2.0f;
    public AudioSource audioSource;
    public AudioClip ClipJump, ClipLanding, ClipFall, ClipCrash;
    Rigidbody2D rb2;
    GameManager gameManager;

    void Start()
    {
        rb2 = GetComponent<Rigidbody2D>();
        gameManager = FindObjectOfType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        float xoff = Input.GetAxis("Horizontal") * speed * Time.deltaTime;
        transform.Translate(xoff, 0.0f, 0.0f);

        if (Input.GetKeyDown(KeyCode.Space) && jumpCount < 2) // 2�� ���� ���
        {
            jumpCount++;
            Vector2 vec2 = rb2.velocity; vec2.y = 0.0f; // y������ ���� ���� �����ϰ��ϴ� �ڵ�
            rb2.velocity = vec2; // x���� �����ӿ� ���� ���ӵ��� ���ְ� jumpForce�� �̿��ϰ��Ѵ�.
            rb2.AddForce(new Vector2(0.0f, jumpForce));
            playJumpSound();
        } 
        else if (Input.GetKeyUp(KeyCode.Space))
        {
            Vector2 vec2 = rb2.velocity; vec2.y *= 0.5f; // y�� �ӵ��� ������ ����
            rb2.velocity = vec2;
        }

        if (gameManager.heart == 0)
        {
            Invoke("gameOver", GameOverTime);
        }

    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Ground") //���鿡 ��Ҵ°�?
        {
            ContactPoint2D pt2 = collision.GetContact(0); 
            if (pt2.normal.y > 0.9f) // ���� ������ �����ΰ�?
            {
                playLandingSound();
                if (jumpCount > 0)
                {
                    gameManager.incScore();
                    jumpCount = 0;
                }
            }
        }
        if (collision.gameObject.tag == "Vine")
        {
            playCrashSound();
            gameManager.decHeart();
            jumpCount = 0;

        }
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.tag == "Death") // ���� ����
        {
            playFallSound();
            Invoke("gameOver", GameOverTime);
        }
    }
    void gameOver()
    {
        SceneManager.LoadScene("GameOver");
    }

    void playJumpSound()
    {
        audioSource.PlayOneShot(ClipJump, 1.0f);
    }
    void playLandingSound()
    {
        audioSource.PlayOneShot(ClipLanding, 1.0f);
    }

    void playFallSound()
    {
        audioSource.PlayOneShot(ClipFall, 1.0f);
    }

    void playCrashSound()
    {
        audioSource.PlayOneShot(ClipCrash, 1.0f);
    }


}
