using NUnit.Framework;
using UnityEngine;
using System.Collections.Generic;
using System.Collections;

public class MakeMole : MonoBehaviour
{
    [SerializeField]
    List<GameObject> moles = new List<GameObject>();

    public float startTime = 1;
    public float repeatTimeMin = 4;
    public float repeatTimeMax = 4;

    public float xmin = -8;
    public float xmax = 8;
    public float zmin = -8;
    public float zmax = 8;

    void Start()
    {
        StartCoroutine("makeMole");
    }



    IEnumerator makeMole()
    {
        yield return new WaitForSeconds(startTime);


        while (true)
        {
            float xpos = Random.Range(xmin, xmax);
            float zpos = Random.Range(zmin, zmax);
            Vector3 pos = transform.position;
            pos.x = xpos;
            pos.y = -2;
            pos.z = zpos;
            Instantiate(moles[Random.Range(0, moles.Count)], pos, transform.rotation);

            yield return new WaitForSeconds(Random.Range(repeatTimeMin, repeatTimeMax));
        }
        
    }


}
