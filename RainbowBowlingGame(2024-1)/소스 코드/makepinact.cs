using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class makepinact : MonoBehaviour
{
    GameManager gameManager;
    public GameObject pin;
    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
        InvokeRepeating("makePin", 1.0f , 5.0f);
    }

    // Update is called once per frame
    void Update()
    {
         if (transform.position.y < -5.0f)
            {
                Destroy(gameObject);
            }
    }
    void makePin()
    {
        Vector3 vector3 = new Vector3();
        vector3.x = Random.Range(-8.0f, 8.0f); 
        vector3.y = Random.Range(5.0f, 10.0f);
        vector3.z = Random.Range(0.0f, 5.0f);
        
        GameObject newPin = Instantiate(pin, vector3, transform.rotation); 
        Renderer rend = newPin.GetComponent<Renderer>();
        rend.material.color = gameManager.getRandomColor();
    }
}


