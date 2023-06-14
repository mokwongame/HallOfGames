using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveCannon : MonoBehaviour
{
    public float speed = 10.0f; 
    GameManager manager;
    // Start is called before the first frame update
    void Start()
    {
        manager = FindObjectOfType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        float xoff = Input.GetAxis("Horizontal") * speed * Time.deltaTime;
        transform.Translate(xoff, 0, 0);
        Vector3 position = transform.position;
        float xpos = position.x;
        xpos = Mathf.Clamp(xpos, -7.0f, 11.0f);
        position.x = xpos;
        transform.position = position;
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Ball")
        {
            manager.outScore();
            Destroy(collision.gameObject);
        }
    }
}
