using System.Collections;
using System.Collections.Generic;
// using UnityEditor.SearchService;
// using UnityEditor.Build.Content;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;
using UnityEditor;

public class MoveCart : MonoBehaviour
{
    public float speed = 20.0f;
    public AudioSource audioSource;
    public AudioClip clipSuccess;
    public AudioClip clipFail;
    public AudioClip CartColorS;
    public float timeScale;
    bool Continue;

    int CartColor = 0;
    GameManager gameManager;
    Vector3 m_MoveMaxLimit = new Vector3(13.0f, 0.0f, 13.0f);

    // Start is called before the first frame update
    void Start()
    {
        Continue = false;
        gameManager = FindObjectOfType<GameManager>();

        Renderer CartRend = gameObject.GetComponent<Renderer>();
        CartRend.material.color = RandomballLife.colors[CartColor];
    }

    // Update is called once per frame
    void Update()
    {
        transform.localPosition = MovePosition(transform.localPosition);



        if (Input.GetKeyDown(KeyCode.Space))
        {
            CartColor++;
            CartColor = CartColor % RandomballLife.colors.Length;
            Renderer Rend = gameObject.GetComponent<Renderer>();
            Rend.material.color = RandomballLife.colors[CartColor];
            CartColorChangeSound();

        }
        else if (Input.GetKeyDown(KeyCode.Escape))
        {
            SceneManager.LoadScene("GameEndScene");
        }
        // float xmoveff = Input.GetAxisRaw("Horizontal") * speed * Time.deltaTime;
        // float zmoveff = Input.GetAxisRaw("Vertical") * speed * Time.deltaTime;
        // gameObject.transform.Translate(xmoveff, 0.0f, zmoveff);
        
        // Test 001 
        float xoff = Input.GetAxis("Horizontal") * speed * Time.deltaTime;
        float zoff = Input.GetAxis("Vertical") * speed * Time.deltaTime;
        Vector3 vecOff = xoff * transform.right - zoff * transform.forward;
        transform.Translate(vecOff);
        
        if(Input.GetKeyDown(KeyCode.Tab))
        {
            if(Continue == false)
            {
                ContinueTrue();
                

            }
            else if (Continue == true)
            {
                ContinueFalse();
                    
            }
        }
        
    }
    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Ball")
        {
            Renderer BallRend = collision.gameObject.GetComponent<Renderer>();
            Color BallColor = BallRend.material.color;

            if (BallColor == RandomballLife.colors[CartColor])
            {
                gameManager.IncScore();
                PlayCartEatScore();
            }

            else
            {
                gameManager.DecScore();
                PlayCartFailScore();
            }
            Debug.Log("Score = " + gameManager.GetScore());
            Destroy(collision.gameObject, 0.0f);
        }

    }
    public Vector3 MovePosition(Vector3 position)
    {
        return new Vector3(Mathf.Clamp(position.x, -m_MoveMaxLimit.x, m_MoveMaxLimit.x),
            0.5f, Mathf.Clamp(position.z, -m_MoveMaxLimit.z, m_MoveMaxLimit.z));
    }
    void PlayCartEatScore()
    {
        audioSource.volume = 1.0f;
        audioSource.PlayOneShot(clipSuccess);
    }
    void PlayCartFailScore()
    {
        audioSource.volume = 1.0f;
        audioSource.PlayOneShot(clipFail);
    }
    void CartColorChangeSound()
    {
        audioSource.volume = 0.5f;
        audioSource.PlayOneShot(CartColorS);
    }

    void ContinueTrue()
    {
        Continue = true;
        Debug.Log("Game Continue");
        Time.timeScale = 0f;

        
    }

    void ContinueFalse()
    {
        Continue = false;

        Debug.Log("Game Start");
        Time.timeScale = 1f;

    }
}
