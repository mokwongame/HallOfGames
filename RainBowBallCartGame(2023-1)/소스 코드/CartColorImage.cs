using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;

public class CartColorImage : MonoBehaviour
{
    int CartColor = 0;
    GameManager gameManager;

    // Start is called before the first frame update
    void Start()
    {
        Renderer CartRend = gameObject.GetComponent<Renderer>();
        CartRend.material.color = RandomballLife.colors[CartColor];
    }

    // Update is called once per frame
    void Update()
    {

        if (Input.GetKeyDown(KeyCode.Space))
        {
            CartColor++;
            CartColor = CartColor % RandomballLife.colors.Length;
            Renderer Rend = gameObject.GetComponent<Renderer>();
            Rend.material.color = RandomballLife.colors[CartColor];

        }
    }
}
