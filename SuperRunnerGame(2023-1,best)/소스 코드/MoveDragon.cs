using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveDragon : MonoBehaviour
{

    public float speed = 5.0f;
    public float dragonBreathing = 1.0f;
    public GameObject breathing;
    public AudioSource audioSource;
    public AudioClip clipDragon;
    GameManager gameManager;
    Rigidbody2D rb2;
    public float power = 50;
    int BreathingCount = 1;
    float width = 14.5f;
    float xmin = 9.0f;
    int leFt = 2;
    int dfcount = 1;



        // Start is called before the first frame update
        void Start()
    {
        rb2 = GetComponent<Rigidbody2D>();
        gameManager = FindObjectOfType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 vec3;

        //transform.Translate(vec3);
        if (transform.position.x >= width)
        {
            leFt = 2;

        }

        if (leFt == 2)
        {
            speed = 1.0f;
            vec3 = Vector3.left * speed * Time.deltaTime;
            transform.Translate(vec3);
            if (transform.position.x <= xmin + 0.03f)
            {
                leFt = 1;

            }

        }
        if (leFt == 1)
        {
            speed = 0.01f;
            vec3 = Vector3.left * speed * Time.deltaTime;
            transform.Translate(vec3);
            if (transform.position.x <= xmin)
            {
                Invoke("BreaThing", dragonBreathing);
                leFt = -1;

            }
        }
        if (leFt == -1)
        {
            
            speed = 0.01f;
            vec3 = Vector3.right * speed * Time.deltaTime;
            transform.Translate(vec3);
            if (transform.position.x <= xmin + 0.03f)
            {
                leFt = -2;

            }

        }
        if (leFt == -2)
        {
            speed = 1.0f;
            vec3 = Vector3.right * speed * Time.deltaTime;
            transform.Translate(vec3);
            Destroy(gameObject, 5.0f);

        }
        
        float xoff = -0.1f * speed * Time.deltaTime; // 상대 위치
        transform.Translate(xoff, 0.0f, 0.0f);
        Vector3 position = transform.position; // 절대 위치
        float xpos = position.x; // xpos를 제한
        xpos = Mathf.Clamp(xpos, 9.0f, 14.5f);
        //if (xpos <= -8.0f) xpos = -8.0f;
        //else if (xpos >= 8.0f) xpos = 8.0f;
        position.x = xpos;
        transform.position = position;
        //Debug.Log(xoff);


        if (BreathingCount < dfcount)
        {
            Breathing();


            BreathingCount++;
        }

        
        }

    void BreaThing()
    {
        BreathingCount = 0;
    }

        void Breathing()
    {

            Vector3 position = transform.position + transform.right * -3.0f + transform.up * -2.0f;
            GameObject DragonBreathing = Instantiate(breathing, position, transform.rotation);
            Rigidbody2D rb = DragonBreathing.GetComponent<Rigidbody2D>();
            rb.AddForce(power * (transform.right * -3.0f + transform.up * -1.4f));

    }

}